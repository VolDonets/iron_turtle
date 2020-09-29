//
// Created by biba_bo on 2020-09-29.
//

#include "smooth_turtle_manager.h"

SmoothTurtleManager::SmoothTurtleManager() {
    rightWheelSpeed = 0.0;
    leftWheelSpeed = 0.0;
    wantedRightWheelSpeed = 0.0;
    wantedLeftWheelSpeed = 0.0;
    //server_count = 0;
    server_count = 5;
    skippingSteps = SPEED_CHANGE_TIME_OUT;
    serialManager = std::make_shared<SerialManager>();
    if (serialManager->isSerialOK()) {
        ironTurtleAPI = std::make_shared<HoverboardAPI>(write_serial_wrapper);
        process_turtle_engines();
    }
#ifdef MY_DEBUG
    else {
        std::cout << "Has a PROBLEM with serial device.\nSo processing DIDN'T STARTED\n";
    }
#endif //MY_DEBUG
}

SmoothTurtleManager::~SmoothTurtleManager() {

}

void SmoothTurtleManager::process_turtle_engines() {
    this->movingProcessingThread = std::thread([this]() {
        while (true) {
            if (server_count > 0) {
                if (skippingSteps == 0) {
                    skippingSteps = SPEED_CHANGE_TIME_OUT;
                    update_current_speed_params();
                    std::cout << "Speed: " << leftWheelSpeed << " " << wantedLeftWheelSpeed << "\n";
                }
                ironTurtleAPI->sendSpeedData(leftWheelSpeed, rightWheelSpeed, 300, 10, PROTOCOL_SOM_NOACK);
                skippingSteps--;
                //server_count--;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_THREAD_TIME_MS));
            if (server_count == -100) //stupid if for idea, newer achieve
                break;                //stupid if for idea
        }
    });
}

void SmoothTurtleManager::update_current_speed_params() {
    if(wantedLeftWheelSpeed > leftWheelSpeed) {
        leftWheelSpeed = leftWheelSpeed + SPEED_CHANGE_STEP_PERCENT;
    } else if (wantedLeftWheelSpeed < leftWheelSpeed) {
        leftWheelSpeed = leftWheelSpeed - SPEED_CHANGE_STEP_PERCENT;
    }



    if(wantedRightWheelSpeed > rightWheelSpeed) {
        rightWheelSpeed = rightWheelSpeed + SPEED_CHANGE_STEP_PERCENT;
    } else if (wantedRightWheelSpeed  < rightWheelSpeed) {
        rightWheelSpeed = rightWheelSpeed - SPEED_CHANGE_STEP_PERCENT;
    }
}

void SmoothTurtleManager::move_faster_left_wheel() {
    if (wantedLeftWheelSpeed >= 0 && wantedLeftWheelSpeed < MAX_SPEED_PERCENT) {
        wantedLeftWheelSpeed = (wantedLeftWheelSpeed + SPEED_CHANGE_STEP_PERCENT > MAX_SPEED_PERCENT) ? MAX_SPEED_PERCENT : (wantedLeftWheelSpeed + SPEED_CHANGE_STEP_PERCENT);
    } else if (wantedLeftWheelSpeed < 0) {
        wantedLeftWheelSpeed = wantedLeftWheelSpeed + SPEED_CHANGE_STEP_PERCENT;
    }
}

void SmoothTurtleManager::move_slower_left_wheel() {
    if (wantedLeftWheelSpeed < 0 && wantedLeftWheelSpeed > -MAX_SPEED_PERCENT) {
        wantedLeftWheelSpeed = (wantedLeftWheelSpeed - SPEED_CHANGE_STEP_PERCENT < -MAX_SPEED_PERCENT) ? -MAX_SPEED_PERCENT : (wantedLeftWheelSpeed - SPEED_CHANGE_STEP_PERCENT);
    } else if (wantedLeftWheelSpeed >= 0) {
        wantedLeftWheelSpeed = wantedLeftWheelSpeed - SPEED_CHANGE_STEP_PERCENT;
    }
}

void SmoothTurtleManager::move_fasted_right_wheel() {
    if (wantedRightWheelSpeed >= 0 && wantedRightWheelSpeed < MAX_SPEED_PERCENT) {
        wantedRightWheelSpeed = (wantedRightWheelSpeed + SPEED_CHANGE_STEP_PERCENT > MAX_SPEED_PERCENT) ? MAX_SPEED_PERCENT : (wantedRightWheelSpeed + SPEED_CHANGE_STEP_PERCENT);
    } else if (wantedRightWheelSpeed < 0) {
        wantedRightWheelSpeed = wantedRightWheelSpeed + SPEED_CHANGE_STEP_PERCENT;
    }
}

void SmoothTurtleManager::move_slower_right_wheel() {
    if (wantedRightWheelSpeed < 0 && wantedRightWheelSpeed > -MAX_SPEED_PERCENT) {
        wantedRightWheelSpeed = (wantedRightWheelSpeed - SPEED_CHANGE_STEP_PERCENT < -MAX_SPEED_PERCENT) ? -MAX_SPEED_PERCENT : (wantedRightWheelSpeed - SPEED_CHANGE_STEP_PERCENT);
    } else if (wantedRightWheelSpeed >= 0) {
        wantedRightWheelSpeed = wantedRightWheelSpeed - SPEED_CHANGE_STEP_PERCENT;
    }
}

void SmoothTurtleManager::say_server_here() {
    server_count = SERVER_PERIODS;
}


void SmoothTurtleManager::move_forward() {
    move_faster_left_wheel();
    move_fasted_right_wheel();
}

void SmoothTurtleManager::move_backward() {
    move_slower_left_wheel();
    move_slower_right_wheel();
}

void SmoothTurtleManager::move_righter() {
    move_faster_left_wheel();
    move_slower_right_wheel();
}

void SmoothTurtleManager::move_lefter() {
    move_slower_left_wheel();
    move_fasted_right_wheel();
}

void SmoothTurtleManager::stop_moving() {
    wantedLeftWheelSpeed = 0.0;
    wantedRightWheelSpeed = 0.0;
    leftWheelSpeed = 0.0;
    rightWheelSpeed = 0.0;
    server_count = 2;
}

int SmoothTurtleManager::get_speed() {
    return 0;
}

int SmoothTurtleManager::get_battery_voltage() {
    return 0;
}

