//
// Created by biba_bo on 2020-08-21.
//

#ifndef IRON_TURTLE_ASCII_NATIVE_DRIVER_POSN_SAVE_H
#define IRON_TURTLE_ASCII_NATIVE_DRIVER_POSN_SAVE_H

#include <cmath>
#include <memory>

#include "ascii_com_lim.h"
#include "serial_manager.h"

class ASCII_NativeDriverCordSave{
private:
    std::shared_ptr<SerialManager> serialDev;

    int enable_immediate_commands();
    int disable_driver();

public:
    ASCII_NativeDriverCordSave();
    ASCII_NativeDriverCordSave(std::shared_ptr<SerialManager> serialDev);
    ~ASCII_NativeDriverCordSave();

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


#endif //IRON_TURTLE_ASCII_NATIVE_DRIVER_POSN_SAVE_H
