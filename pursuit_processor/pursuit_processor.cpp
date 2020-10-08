//
// Created by biba_bo on 2020-10-05.
//

#include "pursuit_processor.h"

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
    return START_DISTANCE_TO_AN_OBJECT * ((Sroi_1 / Sroi_0) - 1);
}

void PursuitProcessor::process_pursuit() {
    this->movingProcessingThread = std::thread([this]() {

        while (isProcessThread) {

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
        process_pursuit();
        return SUCCESSFUL_OP;
    } else {
        return SERIAL_MANAGER_PROBLEM;
    }
}

bool PursuitProcessor::is_process_moving() {
    return isProcessThread;
}

void PursuitProcessor::stop_moving() {

}

void PursuitProcessor::say_server_here() {
    serverCounter = SERVER_WAIT_STEPS;
}

void PursuitProcessor::say_server_leave() {
    stop_moving();
    serverCounter = 20;
}
