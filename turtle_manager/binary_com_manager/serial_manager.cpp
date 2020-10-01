//
// Created by biba_bo on 2020-08-21.
//

#include "serial_manager.h"


int write_serial_wrapper(unsigned char *data, int len) {
    return write(serialPort, data, len);
}

int read_serial_wrapper(unsigned char *data, int len) {
    return UNSUCCESSFUL_OP;
}

SerialManager::SerialManager() {
    activate_serial_port();
}

int SerialManager::activate_serial_port() {
    serialPort = open(DRIVE_FILE_PATH.c_str(), O_RDWR);
    if (!serialPort) {
        if (errno == 2) {
            #ifdef MY_DEBUG
                std::cout << "DEVICE NOT CONNECTED\n";
            #endif //MY_DEBUG
            return SERIAL_DEV_NOT_CONNECTED;
        }
        if (errno == 13) {
            #ifdef MY_DEBUG
                std::cout << "PERMISSION TO DRIVE FILE DENIED\n";
            #endif //MY_DEBUG
            return SERIAL_DEV_PERMISSION_ERR;
        }
        #ifdef MY_DEBUG
            std::cout << "OTHER TYPE OF ERROR\n";
        #endif //MY_DEBUG
        return SERIAL_DEV_OTHER_ERR;
    }

    struct termios tty;                     // create new termios struct, we call it 'tty' for convention
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(serialPort, &tty) != 0) {
        #ifdef MY_DEBUG
            std::cout << "CONFIGURE ERROR\n";
        #endif //MY_DEBUG
        return SERIAL_DEV_CONFIG_ERR;
    }

    tty.c_cflag = B115200 | CS8 | CLOCAL | CREAD;		//<Set baud rate
    tty.c_iflag = IGNPAR;
    tty.c_oflag = 0;
    tty.c_lflag = 0;
    tcflush(serialPort, TCIFLUSH);
    tcsetattr(serialPort, TCSANOW, &tty);

    // saving termios
    if (tcsetattr(serialPort, TCSANOW, &tty) != 0){
        #ifdef MY_DEBUG
            std::cout << "CONFIGURE ERROR\n";
        #endif //MY_DEBUG
        return SERIAL_DEV_CONFIG_ERR;
    }

    return SERIAL_DEV_ACTIVATED;
}



int SerialManager::disable_connection() {
    if(!serialPort) {
        return SUCCESSFUL_OP;
    }
    close(serialPort);
    return SUCCESSFUL_OP;
}

SerialManager::~SerialManager() {
    disable_connection();
}

bool SerialManager::isSerialOK() {
    if (serialPort) {
        return true;
    }
    return false;
}
