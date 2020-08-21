//
// Created by biba_bo on 2020-08-20.
//

#include "turtle_manager.h"

TurtleManager::TurtleManager() {
    speedMod = MOVING_STEP_5;
    rotationMod = MOVING_STEP_5;
    //nativeDriver = std::make_shared<ASCII_NativeDriver>();
    nativeDriver = std::make_shared<ASCII_NativeDriverCordSave>();
}

TurtleManager::TurtleManager(int speedMod, int rotationMod) {
    this->speedMod = speedMod;
    this->rotationMod = rotationMod;
    //nativeDriver = std::make_shared<ASCII_NativeDriver>();
    nativeDriver = std::make_shared<ASCII_NativeDriverCordSave>();
}

int TurtleManager::move_forward() {
    return nativeDriver->move_forward(speedMod);
}

int TurtleManager::move_backward() {
    return nativeDriver->move_back(speedMod);
}

int TurtleManager::move_righter() {
    return nativeDriver->move_righter(rotationMod);
}

int TurtleManager::move_lefter() {
    return nativeDriver->move_lefter(rotationMod);
}
