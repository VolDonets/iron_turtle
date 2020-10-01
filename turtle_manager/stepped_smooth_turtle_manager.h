//
// Created by biba_bo on 2020-10-01.
//

#ifndef IRON_TURTLE_STEPPED_SMOOTH_TURTLE_MANAGER_H
#define IRON_TURTLE_STEPPED_SMOOTH_TURTLE_MANAGER_H

#include <memory>           //used for class shared_ptr<> and make_shared<>()
#include <thread>           //used for a thread creating
#include <atomic>           //used for making class fields as atomic variables for the threads

#include "binary_com_manager/serial_manager.h"                  //this .h-file contains my wrapper for the serial device
#include "binary_com_manager/bipropellant-api/HoverboardAPI.h"  //this .h-file contains C++ class - wrapper for the C bipropellant API

/** The class SteppedSmoothTurtleManager is controller for the turtle engines throw the turtle engine controller with using the binary protocol.
 * !!!WARNING!!! in your program please use only one object of this class, cause in constructor creates wrapper for the serial interface and
 * creates new thread for sending commands throw UART connection to the turtle engine controller.
 * It sends commands only if a customer sends command. Customer can set the max speed for the iron turtle, and it will smoothly achieve it, only
 * if customer push appropriate button.*/
class SteppedSmoothTurtleManager {
private:
    /** This field contains status of the smooth moving start (if it equals true the iron turtle begins moving smoothly and
     * achieves the max wanted speed in the smooth way, otherwise the iron turtle achieve maximum speed immediately.*/
    std::atomic<bool> isSmoothStart;
    /** This field contains status of the smooth moving stopping (if it equals true the iron turtle stops moving smoothly and
     * achieves zero speed in the smooth way, otherwise the iron turtle achieve zero speed immediately.*/
    std::atomic<bool> isSmoothStop;
    /** This field contains speed percent for the wheel steering*/
    std::atomic<double> wheelsSteer;
    /** This field contains wanted speed for the iron turtle*/
    std::atomic<double> wantedSpeed;
    /** This field contains the current left wheel speed */
    std::atomic<double> leftWheelSpeed;
    /** This field contains the current right wheel speed */
    std::atomic<double> rightWheelSpeed;

public:
    SteppedSmoothTurtleManager();
    ~SteppedSmoothTurtleManager();

    void enable_smooth_starting();
    void disable_smooth_starting();

    void enable_smooth_stopping();
    void disable_smooth_stopping();
};


#endif //IRON_TURTLE_STEPPED_SMOOTH_TURTLE_MANAGER_H
