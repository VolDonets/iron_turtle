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
#include "../process_camera/rear_sight_webrtc_manipulation.h"   //this .h-file contains the function for updating a current speed value on the frame

/** @brief this constant means how many steps should to wait a server thread before it decides, that client leave a connection
 *         that means we should to stop sending a commands.*/
constexpr int SERVER_WAIT_STEPS = 10;
/// @brief this constant is a code of the successful operation
constexpr int SUCCESSFUL_OPERATION = 0;
/// @brief this constant is s code of the unsuccessful operation - the reason is problems with connection to the serial devise.
constexpr int SERIAL_MANAGER_PROBLEM = -1;

/** @brief The class SmoothTurtleManager is controller for the turtle engines throw the turtle engine controller with using the binary protocol.
 *  @warning In your program please use only one object of this class, cause in constructor creates wrapper for the serial interface and
 *          creates new thread for sending commands throw UART connection to the turtle engine controller.*/
class SmoothTurtleManager {
private:
    /// @brief this variable contains PERCENT speed for the RIGHT turtle wheel, which we want to achieve, and it will achieve in management process.
    std::atomic<double> wantedRightWheelSpeed;
    /// @brief this variable contains PERCENT speed for the LEFT turtle wheel, which we want to achieve, and it will achieve in management process.
    std::atomic<double> wantedLeftWheelSpeed;
    /// @brief this variable contains PERCENT CURRENT speed for the RIGHT turtle wheel
    std::atomic<double> rightWheelSpeed;
    /// @brief this variable contains PERCENT CURRENT speed for the LEFT turtle wheel
    std::atomic<double> leftWheelSpeed;
    /** @brief this variable contains a server connection status (connection with client), if we have a client which is able to
     *         manage the iron turtle, so we can to send commands to the iron turtle controller.
     *         This variable decrease to 0, in the tread loop, and if a client says 'I'm here it will updates to SERVER_WAIT_STEPS,
     *         otherwise the processor won't able to send a control command and the iron turtle just stop moving.*/
    std::atomic<int> serverCounter;
    /** @brief this variable contains a current processing step, if it equals '0' program will process new rightWheelSpeed and
     *         new leftWheelSpeed, as wanted to achieve wantedRightWheelSpeed and wantedLeftWheelSpeed*/
    std::atomic<int> skippingSteps;
    /// @brief this thread used for processing new speed values for the right and the left wheels, and send it to the turtle engine controller
    std::thread movingProcessingThread;
    /// @brief this is a smart pointer to the wrapper for the serial device driver
    std::shared_ptr<SerialManager> serialManager;
    /// @brief this is a smart pointer to the bipropellant API (C++ class wrapper of the C API).
    std::shared_ptr<HoverboardAPI> ironTurtleAPI;

    /// @brief this is a bool variable, which means is now something are processed or no.
    std::atomic<bool> isProcessThread;


    /// @brief this function recalculate the current speed values for the right and the left wheels as wanted to achieve in the wanted values
    void update_current_speed_params();

    /// @brief this function sets a new value for the field wantedLeftWheelSpeed (INCREASE if possible)
    void move_faster_left_wheel();

    /// @brief this function sets a new value for the field wantedLeftWheelSpeed (DECREASE if possible)
    void move_slower_left_wheel();

    /// @brief this function sets a new value for the field wantedRightWheelSpeed (INCREASE if possible)
    void move_fasted_right_wheel();

    /// @brief this function sets a new value for the field wantedRightWheelSpeed (DECREASE if possible)
    void move_slower_right_wheel();

    /// @brief this function starts new thread for the wheel's speed processing and for the sending control commands to the turtle engine controller
    void process_turtle_engines();

public:
    /** @brief this is default constructor, here inits class fields
     *  @warning this doesn't start a new thread for the processing (a bit changed logic).*/
    SmoothTurtleManager();

    /** @brief this is constructor with parameters
     *  @param std::shared_ptr<SerialManager> serialManager - a driver wrapper for the working with a serial  device throw simple interface.
     *  @warning this doesn't start a new thread for the processing (a bit changed logic).*/
    SmoothTurtleManager(std::shared_ptr<SerialManager> serialManager);

    /// this is a default object destructor
    ~SmoothTurtleManager();

    /// @brief a server call this function, when it has active connection with an active client
    void say_server_here();

    /// @brief a server call this function, when it lost an active client
    void say_server_leave();

    /** @brief this function sets new value for the field wantedLeftWheelSpeed (INCREASE if possible) and for the field wantedRightWheelSpeed (INCREASE if possible)
     *         called when client send command to move forward*/
    void move_forward();

    /** @brief this function sets new value for the field wantedLeftWheelSpeed (DECREASE if possible) and for the field wantedRightWheelSpeed (DECREASE if possible)
     *         called when client send command to move backward*/
    void move_backward();

    /** @brief this function sets new value for the field wantedLeftWheelSpeed (INCREASE if possible) and for the field wantedRightWheelSpeed (DECREASE if possible)
     *         called when client send command to move righter*/
    void move_righter();

    /** @brief this function sets new value for the field wantedLeftWheelSpeed (DECREASE if possible) and for the field wantedRightWheelSpeed (INCREASE if possible)
     *         called when client send command to move lefter*/
    void move_lefter();

    /** @brief this function sets set wantedRightWheelSpeed, wantedLeftWheelSpeed and rightWheelSpeed, leftWheelSpeed as zero (0.0) value
     *         called when client send command to STOP moving*/
    void stop_moving();

    /** @brief this function returns current speed
     *  @return returns current speed */
    int get_speed();

    /** @brief this function returns current battery voltage
     * @return returns current battery voltage of the iron turtle battery*/
    int get_battery_voltage();

    /** @brief This function returns code of operation for the STOPPING thread and it trying if possible to stop the processing thread.
     *  @return an operation result code */
    int stop_processing_thread();

    /** @brief This function returns code of operation for the RESTARTING thread and it trying if possible to stop the processing thread.
     *  @return an operation result code */
    int restart_processing_thread();

    /** @brief This function returns status of the processing thread. If a thread works - the function returns 'true', otherwise it returns - 'false'.
     *  @return an operation result code */
    bool is_process_moving();
};


#endif //IRON_TURTLE_SMOOTH_TURTLE_MANAGER_H
