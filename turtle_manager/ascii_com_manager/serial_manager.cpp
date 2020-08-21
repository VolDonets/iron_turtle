//
// Created by biba_bo on 2020-08-21.
//

#include "serial_manager.h"

SerialManager::SerialManager() {
    activate_serial_port();
    enable_ASCII_mod();
}

int SerialManager::writeSerial(std::string command) {
    if (!serialPort)
        UNSUCCESSFUL_OP;
    write(serialPort, command.c_str(), command.size());
    return SUCCESSFUL_OP;
}

std::string SerialManager::readSerial() {
    return std::string();
}

std::string SerialManager::readSerial(std::string command) {
    return std::string();
}

int SerialManager::operator<<(std::string command) {
    return writeSerial(command);
}

int SerialManager::activate_serial_port() {
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

int SerialManager::enable_ASCII_mod() {
    if (!serialPort)
        return UNSUCCESSFUL_OP;
    write(serialPort, ACTIVATE_ASCII_MODE.c_str(), ACTIVATE_ASCII_MODE.size());
    return SUCCESSFUL_OP;
}

int SerialManager::disable_connection() {
    if(!serialPort)
        return SUCCESSFUL_OP;
    write(serialPort, LEAVE_IMMEDIATE_MODE.c_str(), LEAVE_IMMEDIATE_MODE.size());
    close(serialPort);
    return SUCCESSFUL_OP;
}

SerialManager::~SerialManager() {
    disable_connection();
}

bool SerialManager::isSerialOK() {
    if (serialPort)
        return true;
    return false;
}
