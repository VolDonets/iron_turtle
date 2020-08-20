//
// Created by biba_bo on 2020-08-20.
//

#ifndef IRON_TURTLE_TURTLE_MANAGER_H
#define IRON_TURTLE_TURTLE_MANAGER_H

#include <memory>

#include "ascii_com_manager/ascii_com_lim.h"
#include "ascii_com_manager/ascii_native_driver.h"

class TurtleManager {
private:
    std::shared_ptr<ASCII_NativeDriver> nativeDriver;
    int rotationMod;
    int speedMod;

public:
    TurtleManager();
    TurtleManager(int speedMod, int rotationMod);

    int move_forward();
    int move_backward();
    int move_righter();
    int move_lefter();
};


#endif //IRON_TURTLE_TURTLE_MANAGER_H
