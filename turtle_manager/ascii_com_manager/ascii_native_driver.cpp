//
// Created by biba_bo on 2020-08-20.
//

#include "ascii_native_driver.h"

ASCII_NativeDriver::ASCII_NativeDriver() {
    cRightWheelSpeed = 0;
    cLeftWheelSpeed = 0;
    serialDev = std::make_shared<SerialManager>();
    enable_immediate_commands();
}

ASCII_NativeDriver::ASCII_NativeDriver(std::shared_ptr<SerialManager> serialDev) {
    cRightWheelSpeed = 0;
    cLeftWheelSpeed = 0;
    this->serialDev = serialDev;
    enable_immediate_commands();
}

int ASCII_NativeDriver::enable_immediate_commands() {
    return serialDev->writeSerial(IMMEDIATE_MOVING_MODE_SPEED);
}

int ASCII_NativeDriver::stop_moving() {
    if (!serialDev->writeSerial(IMMEDIATE_MOVE_STOP))
        return UNSUCCESSFUL_OP;
    cLeftWheelSpeed = 0;
    cRightWheelSpeed = 0;
    return SUCCESSFUL_OP;
}

int ASCII_NativeDriver::move_forward(int movingMode) {
    if (!serialDev->isSerialOK())
        return UNSUCCESSFUL_OP;
    if (cRightWheelSpeed + movingMode * SPEED_STEP > MAX_FORWARD_SPEED || cLeftWheelSpeed + movingMode * SPEED_STEP > MAX_FORWARD_SPEED)
        return UNSUCCESSFUL_OP;
    switch (movingMode) {
        case MOVING_STEP_1:
            serialDev->writeSerial(MOVE_FORWARD);
            break;
        case MOVING_STEP_5:
            serialDev->writeSerial(MOVE_FORWARD_5);
            break;
        case MOVING_STEP_10:
            serialDev->writeSerial(MOVE_FORWARD_10);
            break;
        default:
            return UNSUCCESSFUL_OP;
    }
    cRightWheelSpeed += movingMode * SPEED_STEP;
    cLeftWheelSpeed += movingMode * SPEED_STEP;
    return SUCCESSFUL_OP;
}

int ASCII_NativeDriver::move_back(int movingMode) {
    if (!serialDev->isSerialOK())
        return UNSUCCESSFUL_OP;
    if ((cRightWheelSpeed - (movingMode * SPEED_STEP) < MAX_BACKWARD_SPEED) || (cLeftWheelSpeed - (movingMode * SPEED_STEP) < MAX_BACKWARD_SPEED))
        return UNSUCCESSFUL_OP;
    switch (movingMode) {
        case MOVING_STEP_1:
            serialDev->writeSerial(MOVE_BACK);
            break;
        case MOVING_STEP_5:
            serialDev->writeSerial(MOVE_BACK_5);
            break;
        case MOVING_STEP_10:
            serialDev->writeSerial(MOVE_BACK_10);
            break;
        default:
            return UNSUCCESSFUL_OP;
    }
    cRightWheelSpeed -= movingMode * SPEED_STEP;
    cLeftWheelSpeed -= movingMode * SPEED_STEP;
    return SUCCESSFUL_OP;
}

int ASCII_NativeDriver::move_righter(int movingMode) {
    if (!serialDev->isSerialOK())
        return UNSUCCESSFUL_OP;
    int nRightWheelSpeed = cRightWheelSpeed - (movingMode * ROTATION_STEP);
    int nLeftWheelSpeed = cLeftWheelSpeed + (movingMode * ROTATION_STEP);

    if ((nRightWheelSpeed < MAX_BACKWARD_SPEED) || (nLeftWheelSpeed > MAX_FORWARD_SPEED) || abs(nRightWheelSpeed - nLeftWheelSpeed) > MAX_DIFF_ROT)
        return UNSUCCESSFUL_OP;

    switch (movingMode) {
        case MOVING_STEP_1:
            serialDev->writeSerial(MOVE_RIGHT);
            break;
        case MOVING_STEP_5:
            serialDev->writeSerial(MOVE_RIGHT_5);
            break;
        case MOVING_STEP_10:
            serialDev->writeSerial(MOVE_RIGHT_10);
            break;
        default:
            return UNSUCCESSFUL_OP;
    }
    cRightWheelSpeed = nRightWheelSpeed;
    cLeftWheelSpeed = nLeftWheelSpeed;
    return SUCCESSFUL_OP;
}

int ASCII_NativeDriver::move_lefter(int movingMode) {
    if (!serialDev->isSerialOK())
        return UNSUCCESSFUL_OP;

    int nRightWheelSpeed = cRightWheelSpeed + (movingMode * ROTATION_STEP);
    int nLeftWheelSpeed = cLeftWheelSpeed - (movingMode * ROTATION_STEP);
    if ((nRightWheelSpeed > MAX_FORWARD_SPEED) || (nLeftWheelSpeed < MAX_BACKWARD_SPEED) || abs(nRightWheelSpeed - nLeftWheelSpeed) > MAX_DIFF_ROT)
        return UNSUCCESSFUL_OP;

    switch (movingMode) {
        case MOVING_STEP_1:
            serialDev->writeSerial(MOVE_LEFT);
            break;
        case MOVING_STEP_5:
            serialDev->writeSerial(MOVE_LEFT_5);
            break;
        case MOVING_STEP_10:
            serialDev->writeSerial(MOVE_LEFT_10);
            break;
        default:
            return UNSUCCESSFUL_OP;
    }
    cRightWheelSpeed = nRightWheelSpeed;
    cLeftWheelSpeed = nLeftWheelSpeed;
    return SUCCESSFUL_OP;
}

int ASCII_NativeDriver::get_battery_voltage() {
    return 0;
}

int ASCII_NativeDriver::get_current_moving_speed() {
    return (cRightWheelSpeed + cLeftWheelSpeed) / 2;
}

int ASCII_NativeDriver::get_current_rotation_speed() {
    return cRightWheelSpeed - cLeftWheelSpeed;
}

int ASCII_NativeDriver::pause_moving() {
    return 0;
}

int ASCII_NativeDriver::resume_moving() {
    return 0;
}

int ASCII_NativeDriver::disable_driver() {
    return serialDev->writeSerial(LEAVE_IMMEDIATE_MODE);
}

ASCII_NativeDriver::~ASCII_NativeDriver() {
    disable_driver();
}
