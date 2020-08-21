//
// Created by biba_bo on 2020-08-21.
//

#ifndef IRON_TURTLE_SERIAL_MANAGER_H
#define IRON_TURTLE_SERIAL_MANAGER_H


#include <iostream>
#include <string.h>
#include <fcntl.h>      // contains file controls like O_RDWR
#include <errno.h>      // error integer and strerror() function
#include <termios.h>    // contains POSIX terminal control definitions
#include <unistd.h>     // write(), read(), close()

#include "ascii_com_lim.h"

class SerialManager {
private:
    int serialPort;

    int activate_serial_port();
    int enable_ASCII_mod();
    int disable_connection();
public:
    SerialManager();
    ~SerialManager();

    bool isSerialOK();

    int writeSerial(std::string command);
    std::string readSerial();
    std::string readSerial(std::string command);

    int operator<<(std::string command);
};


#endif //IRON_TURTLE_SERIAL_MANAGER_H
