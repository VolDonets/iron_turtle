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

int PursuitProcessor::x_offset(const cv::Rect& newPosition) {
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
