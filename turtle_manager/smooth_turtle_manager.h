//
// Created by biba_bo on 2020-09-29.
//

#ifndef IRON_TURTLE_SMOOTH_TURTLE_MANAGER_H
#define IRON_TURTLE_SMOOTH_TURTLE_MANAGER_H

#include <memory>
#include <thread>
#include <atomic>

#include "binary_com_manager/serial_manager.h"
#include "binary_com_manager/bipropellant-api/HoverboardAPI.h"

class SmoothTurtleManager {
private:
    std::atomic<double> wantedRightWheelSpeed;
    std::atomic<double> wantedLeftWheelSpeed;
    std::atomic<double> rightWheelSpeed;
    std::atomic<double> leftWheelSpeed;
    std::atomic<bool> isServerConnected;
    std::atomic<int> skippingSteps;
    std::thread movingProcessingThread;
    std::shared_ptr<SerialManager> serialManager;
    std::shared_ptr<HoverboardAPI> ironTurtleAPI;

    void update_current_speed_params();
    void move_faster_left_wheel();
    void move_slower_left_wheel();
    void move_fasted_right_wheel();
    void move_slower_right_wheel();
    void process_turtle_engines();

public:
    SmoothTurtleManager();
    ~SmoothTurtleManager();

    void say_server_here();
    void say_server_leave();

    void move_forward();
    void move_backward();
    void move_righter();
    void move_lefter();

    void stop_moving();

    int get_speed();
    int get_battery_voltage();
};


#endif //IRON_TURTLE_SMOOTH_TURTLE_MANAGER_H
