//
// Created by biba_bo on 2020-08-20.
//

#include "ascii_native_driver.h"

ASCII_NativeDriver::ASCII_NativeDriver() {
    cRightWheelSpeed = 0;
    cLeftWheelSpeed = 0;
    activate_serial_port();
    enable_ASCII_mod();
    enable_immediate_commands();
}

int ASCII_NativeDriver::activate_serial_port() {
    serialPort = open(DRIVE_FILE_PATH.c_str(), O_RDWR);
    if (!serialPort) {
        if (errno == 2) {
            std::cout << "DEVICE NOT CONNECTED\n";
            return SERIAL_DEV_NOT_CONNECTED;
        }
        if (errno == 13) {
            std::cout << "PERMISSION TO DRIVE FILE DENIED\n";
            return SERIAL_DEV_PERMISSION_ERR;
        }
        std::cout << "OTHER TYPE OF ERROR\n";
        return SERIAL_DEV_OTHER_ERR;
    }

    struct termios tty;                     // create new termios struct, we call it 'tty' for convention
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(serialPort, &tty) != 0) {
        std::cout << "CONFIGURE ERROR\n";
        return SERIAL_DEV_CONFIG_ERR;
    }

    // there is a configuration of CONTROL MODES (c_cflag)
    tty.c_cflag &= ~PARENB;     // clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB;     // clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= CS8;         // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS;    // disable RST/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // turn on READ & ignore ctrl lines (CLOCAL = 1)

    // there is a configuration of LOCAL MODES (c_lflag)
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;       // disable echo
    tty.c_lflag &= ~ECHOE;      // disable erasure
    tty.c_lflag &= ~ECHONL;     // disable new-line echo
    tty.c_lflag &= ~ISIG;       // disable interpretation of INTR, QUIN and SUSP

    // there is a configuration of OUTPUT MODES (c_oflag)
    tty.c_oflag &= ~OPOST; // prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // prevent conversion of newline to carriage return/line feed

    // there is a configuration of VMIN and VTIME (c_cc)
    tty.c_cc[VTIME] = 0;   // there is VMIN = 0, VTIME = 0: no blocking, return immediately with what is available;
    tty.c_cc[VMIN] = 0;

    // there is a configuration of BAUD RATE (input and output speed)
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);
    // saving termios
    if (tcsetattr(serialPort, TCSANOW, &tty) != 0){
        std::cout << "CONFIGURE ERROR\n";
        return SERIAL_DEV_CONFIG_ERR;
    }

    write(serialPort, EMPTY_COMMAND.c_str(), EMPTY_COMMAND.size());
    return SERIAL_DEV_ACTIVATED;
}

int ASCII_NativeDriver::enable_ASCII_mod() {
    if (!serialPort)
        return UNSUCCESSFUL_OP;
    write(serialPort, ACTIVATE_ASCII_MODE.c_str(), ACTIVATE_ASCII_MODE.size());
    return SUCCESSFUL_OP;
}

int ASCII_NativeDriver::enable_immediate_commands() {
    if (!serialPort)
        return UNSUCCESSFUL_OP;
    write(serialPort, IMMEDIATE_MOVING_MODE_SPEED.c_str(), IMMEDIATE_MOVING_MODE_SPEED.size());
    return SUCCESSFUL_OP;
}

int ASCII_NativeDriver::stop_moving() {
    if (!serialPort)
        return UNSUCCESSFUL_OP;
    write(serialPort, IMMEDIATE_MOVE_STOP.c_str(), IMMEDIATE_MOVE_STOP.size());
    cLeftWheelSpeed = 0;
    cRightWheelSpeed = 0;
    return SUCCESSFUL_OP;
}

int ASCII_NativeDriver::move_forward(int movingMode) {
    if (!serialPort)
        return UNSUCCESSFUL_OP;
    if (cRightWheelSpeed + movingMode * SPEED_STEP > MAX_FORWARD_SPEED || cLeftWheelSpeed + movingMode * SPEED_STEP > MAX_FORWARD_SPEED)
        return UNSUCCESSFUL_OP;
    switch (movingMode) {
        case MOVING_STEP_1:
            write(serialPort, MOVE_FORWARD.c_str(), MOVE_FORWARD.size());
            break;
        case MOVING_STEP_5:
            write(serialPort, MOVE_FORWARD_5.c_str(), MOVE_FORWARD_5.size());
            break;
        case MOVING_STEP_10:
            write(serialPort, MOVE_FORWARD_10.c_str(), MOVE_FORWARD_10.size());
            break;
        default:
            return UNSUCCESSFUL_OP;
    }
    cRightWheelSpeed += movingMode * SPEED_STEP;
    cLeftWheelSpeed += movingMode * SPEED_STEP;
    return SUCCESSFUL_OP;
}

int ASCII_NativeDriver::move_back(int movingMode) {
    if (!serialPort)
        return UNSUCCESSFUL_OP;
    if ((cRightWheelSpeed - (movingMode * SPEED_STEP) < MAX_BACKWARD_SPEED) || (cLeftWheelSpeed - (movingMode * SPEED_STEP) < MAX_BACKWARD_SPEED))
        return UNSUCCESSFUL_OP;
    switch (movingMode) {
        case MOVING_STEP_1:
            write(serialPort, MOVE_BACK.c_str(), MOVE_BACK.size());
            break;
        case MOVING_STEP_5:
            write(serialPort, MOVE_BACK_5.c_str(), MOVE_BACK_5.size());
            break;
        case MOVING_STEP_10:
            write(serialPort, MOVE_BACK_10.c_str(), MOVE_BACK_10.size());
            break;
        default:
            return UNSUCCESSFUL_OP;
    }
    cRightWheelSpeed -= movingMode * SPEED_STEP;
    cLeftWheelSpeed -= movingMode * SPEED_STEP;
    return SUCCESSFUL_OP;
}

int ASCII_NativeDriver::move_righter(int movingMode) {
    if (!serialPort)
        return UNSUCCESSFUL_OP;
    int nRightWheelSpeed = cRightWheelSpeed - (movingMode * ROTATION_STEP);
    int nLeftWheelSpeed = cLeftWheelSpeed + (movingMode * ROTATION_STEP);

    if ((nRightWheelSpeed < MAX_BACKWARD_SPEED) || (nLeftWheelSpeed > MAX_FORWARD_SPEED) || abs(nRightWheelSpeed - nLeftWheelSpeed) > MAX_DIFF_ROT)
        return UNSUCCESSFUL_OP;

    switch (movingMode) {
        case MOVING_STEP_1:
            write(serialPort, MOVE_RIGHT.c_str(), MOVE_RIGHT.size());
            break;
        case MOVING_STEP_5:
            write(serialPort, MOVE_RIGHT_5.c_str(), MOVE_RIGHT_5.size());
            break;
        case MOVING_STEP_10:
            std::cout << "interested: " << MOVE_RIGHT_10 << "\n";
            write(serialPort, MOVE_RIGHT_10.c_str(), MOVE_RIGHT_10.size());
            break;
        default:
            return UNSUCCESSFUL_OP;
    }
    cRightWheelSpeed = nRightWheelSpeed;
    cLeftWheelSpeed = nLeftWheelSpeed;
    return SUCCESSFUL_OP;
}

int ASCII_NativeDriver::move_lefter(int movingMode) {
    if (!serialPort)
        return UNSUCCESSFUL_OP;

    int nRightWheelSpeed = cRightWheelSpeed + (movingMode * ROTATION_STEP);
    int nLeftWheelSpeed = cLeftWheelSpeed - (movingMode * ROTATION_STEP);
    if ((nRightWheelSpeed > MAX_FORWARD_SPEED) || (nLeftWheelSpeed < MAX_BACKWARD_SPEED) || abs(nRightWheelSpeed - nLeftWheelSpeed) > MAX_DIFF_ROT)
        return UNSUCCESSFUL_OP;

    switch (movingMode) {
        case MOVING_STEP_1:
            write(serialPort, MOVE_LEFT.c_str(), MOVE_LEFT.size());
            break;
        case MOVING_STEP_5:
            write(serialPort, MOVE_LEFT_5.c_str(), MOVE_LEFT_5.size());
            break;
        case MOVING_STEP_10:
            write(serialPort, MOVE_LEFT_10.c_str(), MOVE_LEFT_10.size());
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
    if(!serialPort)
        return SUCCESSFUL_OP;
    write(serialPort, LEAVE_IMMEDIATE_MODE.c_str(), LEAVE_IMMEDIATE_MODE.size());
    close(serialPort);
    return SUCCESSFUL_OP;
}

ASCII_NativeDriver::~ASCII_NativeDriver() {
    disable_driver();
}
