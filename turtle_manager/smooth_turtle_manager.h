//
// Created by biba_bo on 2020-09-29.
//

#ifndef IRON_TURTLE_SMOOTH_TURTLE_MANAGER_H
#define IRON_TURTLE_SMOOTH_TURTLE_MANAGER_H

#include <memory>           //used for class shared_ptr<> and make_shared<>()
#include <thread>           //used for a thread creating
#include <atomic>           //used for making class fields as atomic variables for the threads

#include "binary_com_manager/serial_manager.h"                  //this .h-file contains my wrapper for the serial device
#include "binary_com_manager/bipropellant-api/HoverboardAPI.h"  //this .h-file contains C++ class - wrapper for the C bipropellant API

constexpr int SERVER_WAIT_STEPS = 10;

/** The class SmoothTurtleManager is controller for the turtle engines throw the turtle engine controller with using the binary protocol.
 * !!!WARNING!!! in your program please use only one object of this class, cause in constructor creates wrapper for the serial interface and
 * creates new thread for sending commands throw UART connection to the turtle engine controller.*/
class SmoothTurtleManager {
private:
    /// this variable contains PERCENT speed for the RIGHT turtle wheel, which we want to achieve, and it will achieve in management process.
    std::atomic<double> wantedRightWheelSpeed;
    /// this variable contains PERCENT speed for the LEFT turtle wheel, which we want to achieve, and it will achieve in management process.
    std::atomic<double> wantedLeftWheelSpeed;
    /// this variable contains PERCENT CURRENT speed for the RIGHT turtle wheel
    std::atomic<double> rightWheelSpeed;
    /// this variable contains PERCENT CURRENT speed for the LEFT turtle wheel
    std::atomic<double> leftWheelSpeed;
    /** this variable contains a server connection status (connection with client), if we have a client which is able to
     * manage the iron turtle, so we can to send commands to the iron turtle controller.
     * This variable decrease to 0, in the tread loop, and if a client says 'I'm here it will updates to SERVER_WAIT_STEPS,
     * otherwise the processor won't able to send a control command and the iron turtle just stop moving.*/
    std::atomic<int> serverCounter;
    /** this variable contains a current processing step, if it equals '0' program will process new rightWheelSpeed and
     * new leftWheelSpeed, as wanted to achieve wantedRightWheelSpeed and wantedLeftWheelSpeed*/
    std::atomic<int> skippingSteps;
    /// this thread used for processing new speed values for the right and the left wheels, and send it to the turtle engine controller
    std::thread movingProcessingThread;
    /// this is a smart pointer to the wrapper for the serial device driver
    std::shared_ptr<SerialManager> serialManager;
    /// this is a smart pointer to the bipropellant API (C++ class wrapper of the C API).
    std::shared_ptr<HoverboardAPI> ironTurtleAPI;


    /// this function recalculate the current speed values for the right and the left wheels as wanted to achieve in the wanted values
    void update_current_speed_params();
    /// this function sets a new value for the field wantedLeftWheelSpeed (INCREASE if possible)
    void move_faster_left_wheel();
    /// this function sets a new value for the field wantedLeftWheelSpeed (DECREASE if possible)
    void move_slower_left_wheel();
    /// this function sets a new value for the field wantedRightWheelSpeed (INCREASE if possible)
    void move_fasted_right_wheel();
    /// this function sets a new value for the field wantedRightWheelSpeed (DECREASE if possible)
    void move_slower_right_wheel();
    /// this function starts new thread for the wheel's speed processing and for the sending control commands to the turtle engine controller
    void process_turtle_engines();

public:
    /** this is default constructor, here inits class fields and starts a new thread for the speed processing
     * (it starts only if possible before was activated successfully serial wrapper*/
    SmoothTurtleManager();
    /// this is a default object destructor
    ~SmoothTurtleManager();

    /// a server call this function, when it has active connection with an active client
    void say_server_here();
    /// a server call this function, when it lost an active client
    void say_server_leave();

    /** this function sets new value for the field wantedLeftWheelSpeed (INCREASE if possible) and for the field wantedRightWheelSpeed (INCREASE if possible)
     * called when client send command to move forward*/
    void move_forward();
    /** this function sets new value for the field wantedLeftWheelSpeed (DECREASE if possible) and for the field wantedRightWheelSpeed (DECREASE if possible)
     * called when client send command to move backward*/
    void move_backward();
    /** this function sets new value for the field wantedLeftWheelSpeed (INCREASE if possible) and for the field wantedRightWheelSpeed (DECREASE if possible)
     * called when client send command to move righter*/
    void move_righter();
    /** this function sets new value for the field wantedLeftWheelSpeed (DECREASE if possible) and for the field wantedRightWheelSpeed (INCREASE if possible)
     * called when client send command to move lefter*/
    void move_lefter();

    /** this function sets set wantedRightWheelSpeed, wantedLeftWheelSpeed and rightWheelSpeed, leftWheelSpeed as zero (0.0) value
     * called when client send command to STOP moving*/
    void stop_moving();

    /// this function returns current speed
    int get_speed();
    /// this function returns current battery voltage
    int get_battery_voltage();
};


#endif //IRON_TURTLE_SMOOTH_TURTLE_MANAGER_H
