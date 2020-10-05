//
// Created by biba_bo on 2020-10-05.
//

#include "pursuit_processor.h"

PursuitProcessor::PursuitProcessor() {

}

PursuitProcessor::PursuitProcessor(std::shared_ptr<SerialManager> serialManager) {

}

PursuitProcessor::~PursuitProcessor() {

}

void PursuitProcessor::update_current_speed_params() {

}

void PursuitProcessor::move_faster_left_wheel() {

}

void PursuitProcessor::move_slower_left_wheel() {

}

void PursuitProcessor::move_fasted_right_wheel() {

}

void PursuitProcessor::move_slower_right_wheel() {

}

void PursuitProcessor::process_pursuit() {

}

int PursuitProcessor::get_speed() {
    return 0;
}

int PursuitProcessor::get_battery_voltage() {
    return 0;
}

int PursuitProcessor::stop_processing_thread() {
    return 0;
}

int PursuitProcessor::restart_processing_thread(cv::Rect fixedRectangleCoord) {
    return 0;
}

bool PursuitProcessor::is_process_moving() {
    return false;
}

void PursuitProcessor::stop_moving() {

}

void PursuitProcessor::say_server_here() {

}

void PursuitProcessor::say_server_leave() {

}
