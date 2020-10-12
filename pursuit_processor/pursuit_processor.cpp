//
// Created by biba_bo on 2020-10-05.
//

#include "pursuit_processor.h"
#include "../process_camera/rear_sight_webrtc_manipulation.h"

PursuitProcessor::PursuitProcessor() {
    // creates a new serial wrapper for connecting to the iron turtle engines controller
    serialManager = std::make_shared<SerialManager>();
    // creates a new object for the speaking with the iron turtle engines controller
    ironTurtleAPI = std::make_shared<HoverboardAPI>(write_serial_wrapper);
    // set a processing server stata (now it doesn't work).
    isProcessThread = false;
}

PursuitProcessor::PursuitProcessor(std::shared_ptr<SerialManager> serialManager) {
    serialManager = serialManager;
    // creates a new object for the speaking with the iron turtle engines controller
    ironTurtleAPI = std::make_shared<HoverboardAPI>(write_serial_wrapper);
    // set a processing server stata (now it doesn't work).
    isProcessThread = false;
}

PursuitProcessor::~PursuitProcessor() {
    // now here nothing to do.
}

double PursuitProcessor::x_offset(const cv::Rect& newPosition) {
    // here we take a new position by x and plus a half of the object width (this means
    // we calculate an object position, as absolute value), than from it we minus
    // old position by x and minus a half of the old object width
    return newPosition.x + (newPosition.width / 2) - fixedRectangleCoord.x - (fixedRectangleCoord.width / 2);
}

double PursuitProcessor::y_delta_moving(const cv::Rect& newPosition) {
    // Here we calculate a new object area and we calculate an old object area,
    // than using a ration between area and roi's areas we can get a distance between
    // the old object and the new object
    double Sroi_0, Sroi_1;
    Sroi_0 = fixedRectangleCoord.width * fixedRectangleCoord.height;
    Sroi_1 = newPosition.width * newPosition.height;
    return START_DISTANCE_TO_AN_OBJECT * sqrt(Sroi_0 / Sroi_1);
}

void PursuitProcessor::process_pursuit() {
    this->movingProcessingThread = std::thread([this]() {
        double xOffset = 0.0;
        double yOffset = 0.0;
        double phiOffset = 0.0;

        int currentPower = 0;
        double leftWheelSpeed = 0.0;
        double rightWheelSpeed = 0.0;
        int sendZeroSpeedTimes = 0;
        constexpr int MAX_ZERO_SPEED_SEND_TIMES = 5;

        while (isProcessThread) {
            if (serverCounter > 0) {
                if (!newRectangleCoordsList.empty()) {
                    cv::Rect newRectPos = newRectangleCoordsList.front();
                    newRectangleCoordsList.pop_front();
                    xOffset = x_offset(newRectPos);
                    yOffset = y_delta_moving(newRectPos);
                    phiOffset = xOffset / PIXELS_PER_ANGLE_RPI_CAM_V2;
                }
                if (abs(xOffset) >= 0) {
                    double lRotation = abs(phiOffset) * SMALL_ROTATION_RADIUS;

                    int accelerationSteps = 0;
                    int continuingSteps = 0;
                    if (MAX_ACCELERATION_DISTANCE_MM > (lRotation / 2)) {
                        accelerationSteps = (sqrt(lRotation / MAX_ACHIEVED_ACCELERATION_MM) / TIME_INTERVAL_BETWEEN_SENDING_COMMANDS);
                    } else {
                        accelerationSteps = (sqrt(MAX_ACCELERATION_DISTANCE_MM / MAX_ACHIEVED_ACCELERATION_MM) / TIME_INTERVAL_BETWEEN_SENDING_COMMANDS);
                        continuingSteps = ((lRotation - (2 * MAX_ACCELERATION_DISTANCE_MM)) / MAX_ACHIEVED_SPEED_MM) / TIME_INTERVAL_BETWEEN_SENDING_COMMANDS;
                    }

                    for (int inx = 0; inx < 2; inx++) {
                        ironTurtleAPI->sendSpeedData(0.0, 0.0, 0, 0, PROTOCOL_SOM_NOACK);
                        std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_THREAD_TIME_MS));
                    }

                    for (int inx = 0; inx < accelerationSteps; inx++) {
                        leftWheelSpeed += MAX_ACHIEVED_ACCELERATION_PERCENT;
                        rightWheelSpeed += MAX_ACHIEVED_ACCELERATION_PERCENT;
                        currentPower = (currentPower < 200) ? (currentPower + 50) : 200;
                        ironTurtleAPI->sendSpeedData(leftWheelSpeed, rightWheelSpeed, currentPower, 5, PROTOCOL_SOM_NOACK);
                        std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_THREAD_TIME_MS));
                    }
                    for (int inx = 0; inx < continuingSteps; inx++) {
                        ironTurtleAPI->sendSpeedData(MAX_ACHIEVED_SPEED_PERCENT, MAX_ACHIEVED_SPEED_PERCENT, 200, 5, PROTOCOL_SOM_NOACK);
                        std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_THREAD_TIME_MS));
                    }
                    for (int inx = 0; inx < accelerationSteps; inx++) {
                        leftWheelSpeed -= MAX_ACHIEVED_ACCELERATION_PERCENT;
                        rightWheelSpeed -= MAX_ACHIEVED_ACCELERATION_PERCENT;
                        currentPower = (currentPower < 200) ? (currentPower + 50) : 200;
                        ironTurtleAPI->sendSpeedData(leftWheelSpeed, rightWheelSpeed, currentPower, 5, PROTOCOL_SOM_NOACK);
                        std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_THREAD_TIME_MS));
                        sendZeroSpeedTimes = 0;
                    }

#ifdef MY_PURSUIT_TESTING
                    std::string message = "Dx: " + std::to_string((int)xOffset)
                            + ", accSt: " + std::to_string(accelerationSteps)
                            + ", contSt: " + std::to_string(continuingSteps);

                    set_debug_data_message(message);
#endif //MY_PURSUIT_TESTING
                }
            } else {
                while (sendZeroSpeedTimes < MAX_ZERO_SPEED_SEND_TIMES) {
                    ironTurtleAPI->sendSpeedData(0.0, 0.0, 0, 0, PROTOCOL_SOM_NOACK);
                    std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_THREAD_TIME_MS));
                    sendZeroSpeedTimes++;
                }
            }
            serverCounter = (serverCounter > 0) ? (serverCounter - 1) : 0;
            std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_THREAD_TIME_FOR_FRAME_MS));
        }
    });
}

int PursuitProcessor::get_speed() {
    return 0;
}

int PursuitProcessor::get_battery_voltage() {
    return 0;
}

int PursuitProcessor::stop_processing_thread() {
    if (isProcessThread) {
        isProcessThread = false;
        stop_processing_thread();
        std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_THREAD_TIME_MS));
        ironTurtleAPI->sendSpeedData(0.0, 0.0, 0, 0, PROTOCOL_SOM_NOACK);
    }
    return SUCCESSFUL_OP;
}

int PursuitProcessor::restart_processing_thread(cv::Rect fixedRectangleCoord) {
    if (isProcessThread) {
        return SUCCESSFUL_OP;
    }
    if (serialManager->isSerialOK()) {
        isProcessThread = true;
        this->fixedRectangleCoord = fixedRectangleCoord;
        process_pursuit();
        return SUCCESSFUL_OP;
    } else {
        return P_SERIAL_MANAGER_PROBLEM;
    }
}

bool PursuitProcessor::is_process_moving() {
    return isProcessThread;
}

void PursuitProcessor::stop_moving() {

}

void PursuitProcessor::say_server_here() {
    serverCounter = P_SERVER_WAIT_STEPS;
}

void PursuitProcessor::say_server_leave() {
    stop_moving();
    serverCounter = 20;
}

void PursuitProcessor::add_coord_to_queue(cv::Rect newCoord) {
    newRectangleCoordsList.push_back(newCoord);
}
