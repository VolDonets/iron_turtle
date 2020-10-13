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

double PursuitProcessor::y_distance(const cv::Rect& newPosition) {
    // Here we calculate a new object area and we calculate an old object area,
    // than using a ration between area and roi's areas we can get a distance between
    // the old object and the new object
    double Sroi_0, Sroi_1;
    Sroi_0 = fixedRectangleCoord.width * fixedRectangleCoord.height;
    Sroi_1 = newPosition.width * newPosition.height;
    return START_DISTANCE_TO_AN_OBJECT * sqrt(Sroi_0 / Sroi_1);
}

void PursuitProcessor::process_pursuit() {
    // Starting an independent thread for calculating a new position for the iron turtle
    // in accordance with a current position and an aim's position.
    this->movingProcessingThread = std::thread([this]() {
        // this is an offset by X-axis, it defines how big angle should rotate the iron turtle body
        double xOffset = 0.0;
        // this is an offset by Y-axis, it defines how big distance between an object and the iron turtle body
        double yOffset = 0.0;
        // this is an angle for rotation
        double phiOffset = 0.0;

        // this variable defines a speed changing, it uses for smooth moving
        int currentPower = 0;
        // this variable contains current speed for the left wheel in percent from the maximum speed
        double leftWheelSpeed = 0.0;
        // this variable contains current speed for the right wheel in percent from the maximum speed
        double rightWheelSpeed = 0.0;
        // this variable uses for minimazing jerks, for sending zero speed status
        int sendZeroSpeedTimes = 0;
        // this constant uses for setting the maximum count of sending zero speed status
        constexpr int MAX_ZERO_SPEED_SEND_TIMES = 5;

        // the 'infinite' loop for processing the iron turtle moving in according with a current position and an aim's position.
        while (isProcessThread) {
            // means if server has a client connection, so we should process moving
            if (serverCounter > 0) {
                // if we has a rectangle of detected object
                if (!newRectangleCoordsList.empty()) {
                    cv::Rect newRectPos = newRectangleCoordsList.front();
                    newRectangleCoordsList.pop_front();
                    xOffset = x_offset(newRectPos);
                    yOffset = y_distance(newRectPos) - START_DISTANCE_TO_AN_OBJECT;
                    phiOffset = xOffset / PIXELS_PER_ANGLE_RPI_CAM_V2;
                }
                if (abs(xOffset) > 0) {
                    double lRotation = abs(phiOffset) * SMALL_ROTATION_RADIUS;

                    int accelerationSteps = 0;
                    int continuingSteps = 0;
                    // this code block calculates acceleration steps (when speed increase and decrease),
                    // and continuing steps (when speed has the same value whole time)
                    if (MAX_ACCELERATION_DISTANCE_MM > (lRotation / 2)) {
                        accelerationSteps = (sqrt(lRotation / MAX_ACHIEVED_ACCELERATION_MM) / TIME_INTERVAL_BETWEEN_SENDING_COMMANDS);
                        continuingSteps = 0;
                    } else {
                        accelerationSteps = (sqrt(MAX_ACCELERATION_DISTANCE_MM / MAX_ACHIEVED_ACCELERATION_MM) / TIME_INTERVAL_BETWEEN_SENDING_COMMANDS);
                        continuingSteps = ((lRotation - (2 * MAX_ACCELERATION_DISTANCE_MM)) / MAX_ACHIEVED_SPEED_MM) / TIME_INTERVAL_BETWEEN_SENDING_COMMANDS;
                    }

                    sendZeroSpeedTimes = 0;

                    // ******************************************************************************************* //
                    //                                  ROTATION STAGE
                    // make wheel controller calm down
                    for (int inx = 0; inx < 3; inx++) {
                        ironTurtleAPI->sendSpeedData(0.0, 0.0, 0, 0, PROTOCOL_SOM_NOACK);
                        std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_THREAD_TIME_MS));
                    }

                    // linearly increase wheels speed
                    for (int inx = 0; inx < accelerationSteps; inx++) {
                        leftWheelSpeed = (xOffset > 0) ? (leftWheelSpeed + MAX_ACHIEVED_ACCELERATION_PERCENT) : (leftWheelSpeed - MAX_ACHIEVED_ACCELERATION_PERCENT);
                        rightWheelSpeed = (xOffset > 0) ? (rightWheelSpeed - MAX_ACHIEVED_ACCELERATION_PERCENT) : (rightWheelSpeed + MAX_ACHIEVED_ACCELERATION_PERCENT);
                        currentPower = (currentPower < 200) ? (currentPower + 50) : 200;
                        ironTurtleAPI->sendSpeedData(leftWheelSpeed, rightWheelSpeed, currentPower, 5, PROTOCOL_SOM_NOACK);
                        std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_THREAD_TIME_MS));
                    }

                    // continuing a current speed value
                    for (int inx = 0; inx < continuingSteps; inx++) {
                        ironTurtleAPI->sendSpeedData(MAX_ACHIEVED_SPEED_PERCENT, MAX_ACHIEVED_SPEED_PERCENT, 200, 5, PROTOCOL_SOM_NOACK);
                        std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_THREAD_TIME_MS));
                    }

                    // linearly decrease wheels speed
                    for (int inx = 0; inx < accelerationSteps; inx++) {
                        leftWheelSpeed = (xOffset > 0) ? (leftWheelSpeed - MAX_ACHIEVED_ACCELERATION_PERCENT) : (leftWheelSpeed + MAX_ACHIEVED_ACCELERATION_PERCENT);
                        rightWheelSpeed = (xOffset > 0) ? (rightWheelSpeed + MAX_ACHIEVED_ACCELERATION_PERCENT) : (rightWheelSpeed - MAX_ACHIEVED_ACCELERATION_PERCENT);
                        currentPower = (currentPower < 200) ? (currentPower + 50) : 200;
                        ironTurtleAPI->sendSpeedData(leftWheelSpeed, rightWheelSpeed, currentPower, 5, PROTOCOL_SOM_NOACK);
                        std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_THREAD_TIME_MS));
                    }
                    // ******************************************************************************************* //

                    // this code block calculates acceleration steps (when speed increase and decrease),
                    // and continuing steps (when speed has the same value whole time)
                    if (MAX_ACCELERATION_DISTANCE_MM > abs(yOffset) / 2) {
                        accelerationSteps = (sqrt(abs(yOffset) / MAX_ACHIEVED_ACCELERATION_MM) / TIME_INTERVAL_BETWEEN_SENDING_COMMANDS);
                    } else {
                        accelerationSteps = (sqrt(MAX_ACCELERATION_DISTANCE_MM / MAX_ACHIEVED_ACCELERATION_MM) / TIME_INTERVAL_BETWEEN_SENDING_COMMANDS);
                        continuingSteps = ((abs(yOffset) - (2 * MAX_ACCELERATION_DISTANCE_MM)) / MAX_ACHIEVED_SPEED_MM) / TIME_INTERVAL_BETWEEN_SENDING_COMMANDS;
                    }

                    // ******************************************************************************************* //
                    //                                  MOVING TO THE AIM STAGE
                    // make wheel controller calm down
                    for (int inx = 0; inx < 3; inx++) {
                        ironTurtleAPI->sendSpeedData(0.0, 0.0, 0, 0, PROTOCOL_SOM_NOACK);
                        std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_THREAD_TIME_MS));
                    }

                    // linearly increase wheels speed
                    for (int inx = 0; inx < accelerationSteps; inx++) {
                        leftWheelSpeed = (yOffset > 0) ? (leftWheelSpeed + MAX_ACHIEVED_ACCELERATION_PERCENT) : (leftWheelSpeed - MAX_ACHIEVED_ACCELERATION_PERCENT);
                        rightWheelSpeed = (yOffset > 0) ? (rightWheelSpeed + MAX_ACHIEVED_ACCELERATION_PERCENT) : (rightWheelSpeed - MAX_ACHIEVED_ACCELERATION_PERCENT);
                        currentPower = (currentPower < 200) ? (currentPower + 50) : 200;
                        ironTurtleAPI->sendSpeedData(leftWheelSpeed, rightWheelSpeed, currentPower, 5, PROTOCOL_SOM_NOACK);
                        std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_THREAD_TIME_MS));
                    }

                    // continuing a current speed value
                    for (int inx = 0; inx < continuingSteps; inx++) {
                        ironTurtleAPI->sendSpeedData(MAX_ACHIEVED_SPEED_PERCENT, MAX_ACHIEVED_SPEED_PERCENT, 200, 5, PROTOCOL_SOM_NOACK);
                        std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_THREAD_TIME_MS));
                    }

                    // linearly decrease wheels speed
                    for (int inx = 0; inx < accelerationSteps; inx++) {
                        leftWheelSpeed = (yOffset > 0) ? (leftWheelSpeed - MAX_ACHIEVED_ACCELERATION_PERCENT) : (leftWheelSpeed + MAX_ACHIEVED_ACCELERATION_PERCENT);
                        rightWheelSpeed = (yOffset > 0) ? (rightWheelSpeed - MAX_ACHIEVED_ACCELERATION_PERCENT) : (rightWheelSpeed + MAX_ACHIEVED_ACCELERATION_PERCENT);
                        currentPower = (currentPower < 200) ? (currentPower + 50) : 200;
                        ironTurtleAPI->sendSpeedData(leftWheelSpeed, rightWheelSpeed, currentPower, 5, PROTOCOL_SOM_NOACK);
                        std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_THREAD_TIME_MS));
                    }
                    // ******************************************************************************************* //

#ifdef MY_PURSUIT_TESTING
                    std::string message = "Dy: " + std::to_string((int)yOffset)
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
