//
// Created by biba_bo on 2020-08-19.
//



#include <memory>
#include "turtle_manager/ascii_com_manager/ascii_com_lim.h"
#include "turtle_manager/ascii_com_manager/ascii_native_driver.h"

int main (int argc, char* argv[]) {
    std::shared_ptr<ASCII_NativeDriver> driver = std::make_shared<ASCII_NativeDriver>();
    std::cout << "Moving forward\n";
    driver->move_righter(MOVING_STEP_10);
    sleep(1);
    driver->move_righter(MOVING_STEP_10);
    sleep(1);
    driver->move_righter(MOVING_STEP_10);
    sleep(1);
    driver->move_righter(MOVING_STEP_10);
    sleep(1);
    driver->move_righter(MOVING_STEP_10);
    sleep(1);
    sleep(10);
    std::cout << "Stop moving\n";
    driver->stop_moving();
    return 0;
}
