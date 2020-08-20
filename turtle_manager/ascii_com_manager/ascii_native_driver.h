//
// Created by biba_bo on 2020-08-20.
//

#ifndef IRON_TURTLE_ASCII_NATIVE_DRIVER_H
#define IRON_TURTLE_ASCII_NATIVE_DRIVER_H

#include <iostream>
#include <string.h>
#include <fcntl.h>      // contains file controls like O_RDWR
#include <errno.h>      // error integer and strerror() function
#include <termios.h>    // contains POSIX terminal control definitions
#include <unistd.h>     // write(), read(), close()
#include <cmath>

#include "ascii_com_lim.h"

class ASCII_NativeDriver {
private:
    int serialPort;
    int cRightWheelSpeed;
    int cLeftWheelSpeed;

    int activate_serial_port();
    int enable_ASCII_mod();
    int enable_immediate_commands();
    int disable_driver();

public:
    ASCII_NativeDriver();
    ~ASCII_NativeDriver();

    int move_forward(int movingMode);
    int move_back(int movingMode);
    int move_righter(int movingMode);
    int move_lefter(int movingMode);

    int stop_moving();
    int pause_moving();
    int resume_moving();

    int get_battery_voltage();
    int get_current_moving_speed();
    int get_current_rotation_speed();
};


#endif //IRON_TURTLE_ASCII_NATIVE_DRIVER_H
