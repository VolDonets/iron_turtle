//
// Created by biba_bo on 2020-08-21.
//

#include "ascii_native_driver_posn_save.h"

ASCII_NativeDriverCordSave::ASCII_NativeDriverCordSave() {
    serialDev = std::make_shared<SerialManager>();
    enable_immediate_commands();
}

ASCII_NativeDriverCordSave::ASCII_NativeDriverCordSave(std::shared_ptr<SerialManager> serialDev) {
    this->serialDev = serialDev;
    enable_immediate_commands();
}

ASCII_NativeDriverCordSave::~ASCII_NativeDriverCordSave() {
    disable_driver();
}

int ASCII_NativeDriverCordSave::move_forward(int movingMode) {
    return serialDev->writeSerial(MOVE_FORWARD);
}

int ASCII_NativeDriverCordSave::move_back(int movingMode) {
    return serialDev->writeSerial(MOVE_BACK);
}

int ASCII_NativeDriverCordSave::move_righter(int movingMode) {
    return serialDev->writeSerial(MOVE_RIGHT);
}

int ASCII_NativeDriverCordSave::move_lefter(int movingMode) {
    return serialDev->writeSerial(MOVE_LEFT);
}

int ASCII_NativeDriverCordSave::stop_moving() {
    return serialDev->writeSerial(IMMEDIATE_MOVE_STOP);
}

int ASCII_NativeDriverCordSave::pause_moving() {
    return UNSUCCESSFUL_OP;
}

int ASCII_NativeDriverCordSave::resume_moving() {
    return UNSUCCESSFUL_OP;
}

int ASCII_NativeDriverCordSave::get_battery_voltage() {
    return 0;
}

int ASCII_NativeDriverCordSave::get_current_moving_speed() {
    return 0;
}

int ASCII_NativeDriverCordSave::get_current_rotation_speed() {
    return 0;
}

int ASCII_NativeDriverCordSave::enable_immediate_commands() {
    return serialDev->writeSerial(IMMEDIATE_MOVING_MODE_POSN);
}

int ASCII_NativeDriverCordSave::disable_driver() {
    return serialDev->writeSerial(LEAVE_IMMEDIATE_MODE);
}
