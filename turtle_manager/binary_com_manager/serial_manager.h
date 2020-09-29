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

#include "binary_com_lim.h"

/// this is a file descriptor for the linux serial file driver
static int serialPort;

/// this is a serial writing wrapper which used in HoverboardAPI
int write_serial_wrapper(unsigned char *data, int len);
/// this is a serial reading wrapper which used in HoverboardAPI
int read_serial_wrapper(unsigned char *data, int len);

/// this class controls connection to the serial device
class SerialManager {
private:
    /// this function activate serial port with preparing serial driver
    int activate_serial_port();
    /// this function deactivate serial port
    int disable_connection();

public:
    /// this is default class constructor which trying to activate a serial port
    SerialManager();
    /// this is default destructor which close a file descriptor to the serial device
    ~SerialManager();

    /// this function check: Is serial device prepared?
    bool isSerialOK();
};


#endif //IRON_TURTLE_SERIAL_MANAGER_H
