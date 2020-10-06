//
// Created by biba_bo on 2020-10-05.
//

#ifndef IRON_TURTLE_PURSUIT_PROCESSOR_H
#define IRON_TURTLE_PURSUIT_PROCESSOR_H

#include <list>
#include <memory>
#include <opencv2/core/types.hpp>


#include "../turtle_manager/binary_com_manager/serial_manager.h"

/** @brief this constant means how many steps should to wait a server thread before it decides, that client leave a connection
 *         that means we should to stop sending a commands.*/
constexpr int SERVER_WAIT_STEPS = 10;
/// @brief this constant is a code of the successful operation
constexpr int SUCCESSFUL_OPERATION = 0;
/// @brief this constant is s code of the unsuccessful operation - the reason is problems with connection to the serial devise.
constexpr int SERIAL_MANAGER_PROBLEM = -1;

/// @brief this is constant is a distance in mm from camera to the object
constexpr double START_DISTANCE_TO_AN_OBJECT = 1000;  // TODO. I should measure it soon.

/** @brief This is class for the processing auto moving of the iron turtle.
 *  This has a start rectangle size and place and recalculate moving direction and speed by a new object rectangle and position.
 *  This recalculations works in other thread and gets data from the process camera thread.
 *  Possibly it can get data from other places.*/
class PursuitProcessor {
private:
    /// @brief this is last fixed coord which was achieved by the iron turtle
    cv::Rect fixedRectangleCoord;
    /// @brief this a queue of the detected object coordinates
    std::list<cv::Rect> newRectangleCoordsList;

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
    void process_pursuit();

    /**
     * @brief this function calculate an object position bias
     * @param newPosition - a cv rectangle with a new object position
     * @return current object bias (if haven't image it returns 0)
     */
    int x_offset(const cv::Rect &newPosition);

    /**
     * @brief this function calculate an distance between an original object position and a new one.
     * @param newPosition - a cv rectangle with a new object position
     * @return distance between an original object position and a new one.
     */
    double y_delta_moving(const cv::Rect &newPosition);

public:
    /** @brief this is a default constructor, here inits all needed fields
     *  @warning this constructor doesn't start new thread so you should manually start it, for activating the pursuit function*/
    PursuitProcessor();

    /** @brief this is a default constructor, here inits NOT all needed fields
     *  @param serialManager - this is a driver wrapper for the working with serial device (here it's connected by UARD
     *  the iron turtle engines controller
     *  @warning this constructor doesn't start new thread so you should manually start it, for activating the pursuit function*/
    PursuitProcessor(std::shared_ptr<SerialManager> serialManager);

    /** @brief this is a default destructor
     *  @warning this one doing nothing */
    ~PursuitProcessor();


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
    int restart_processing_thread(cv::Rect fixedRectangleCoord);

    /** @brief This function returns status of the processing thread. If a thread works - the function returns 'true', otherwise it returns - 'false'.
     *  @return an operation result code */
    bool is_process_moving();

    /** @brief this function sets set wantedRightWheelSpeed, wantedLeftWheelSpeed and rightWheelSpeed, leftWheelSpeed as zero (0.0) value
     *  called when client send command to STOP moving*/
    void stop_moving();

    /// @brief a server call this function, when it has active connection with an active client
    void say_server_here();

    /// @brief a server call this function, when it lost an active client
    void say_server_leave();
};


#endif //IRON_TURTLE_PURSUIT_PROCESSOR_H
