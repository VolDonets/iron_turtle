//
// Created by biba_bo on 2020-10-05.
//

#ifndef IRON_TURTLE_PURSUIT_PROCESSOR_H
#define IRON_TURTLE_PURSUIT_PROCESSOR_H

#include <list>
#include <memory>
#include <opencv2/core/types.hpp>
#include <atomic>
#include <thread>

#include "../turtle_manager/binary_com_manager/serial_manager.h"
#include "../turtle_manager/binary_com_manager/bipropellant-api/HoverboardAPI.h"

/** @brief this constant means how many steps should to wait a server thread before it decides, that client leave a connection
 *         that means we should to stop sending a commands.*/
constexpr int P_SERVER_WAIT_STEPS = 10;
/// @brief this constant is a code of the successful operation
constexpr int P_SUCCESSFUL_OPERATION = 0;
/// @brief this constant is s code of the unsuccessful operation - the reason is problems with connection to the serial devise.
constexpr int P_SERIAL_MANAGER_PROBLEM = -1;

/// @brief this is constant is a distance in 'mm' from camera to the object
constexpr double START_DISTANCE_TO_AN_OBJECT = 700;

/// @brief this constant is a how many pixels takes a RPI camera v2 on each angle in radian
constexpr double PIXELS_PER_ANGLE_RPI_CAM_V2 = 627.450980392;

/// @brief this constant is a small rotation radius (here means the rotation processes both wheels) in 'mm'
constexpr double SMALL_ROTATION_RADIUS = 150;

/// @brief this constant is a maximum acceleration (in mm per s^2) with which can move the iron turtle
constexpr double MAX_ACHIEVED_ACCELERATION_MM = 340;
/// @brief this constant is a maximum speed (in mm per s) with which can move the iron turtle
constexpr double MAX_ACHIEVED_SPEED_MM = 61;
/** @brief this constant is a distance which achieve the iron turtle if will move with maximum acceleration till achieves
 *         the maximum speed. This value given in mm*/
constexpr double MAX_ACCELERATION_DISTANCE_MM = 5.47205;
/// @brief this constant is a maximum time for the doing one iteration in the iron turtle engine controller (in seconds)
constexpr double TIME_INTERVAL_BETWEEN_SENDING_COMMANDS = 0.03;

/// @brief this constant is a maximum acceleration (in percent from max value) with which can move the iron turtle
constexpr double MAX_ACHIEVED_ACCELERATION_PERCENT = 0.005;
/// @brief this constant is a maximum speed (in percent from max value) with which can move the iron turtle
constexpr double MAX_ACHIEVED_SPEED_PERCENT = 0.03;



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
    /** @brief this variable shows should server process new position for the iron turtle
     *         also it means should works a rotation thread (is it works now?)*/
    std::atomic<bool> isProcessThread;
    /// @brief this is a smart pointer to the wrapper for the serial device driver
    std::shared_ptr<SerialManager> serialManager;
    /// @brief this is a smart pointer to the bipropellant API (C++ class wrapper of the C API).
    std::shared_ptr<HoverboardAPI> ironTurtleAPI;
    /// @brief this thread used for processing new speed values for the right and the left wheels, and send it to the turtle engine controller
    std::thread movingProcessingThread;
    /** @brief this variable contains a server connection status (connection with client), if we have a client which is able to
     *         manage the iron turtle, so we can to send commands to the iron turtle controller.
     *         This variable decrease to 0, in the tread loop, and if a client says 'I'm here it will updates to SERVER_WAIT_STEPS,
     *         otherwise the processor won't able to send a control command and the iron turtle just stop moving.*/
    std::atomic<int> serverCounter;

    /// @brief this function starts new thread for the wheel's speed processing and for the sending control commands to the turtle engine controller
    void process_pursuit();

    /**
     * @brief this function calculate an object position bias
     * @param newPosition - a cv rectangle with a new object position
     * @return current object bias (if haven't image it returns 0)
     */
    double x_offset(const cv::Rect &newPosition);

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

    /// @brief this function just add a new Rect object with coordinates
    void add_coord_to_queue(cv::Rect newCoord);
};


#endif //IRON_TURTLE_PURSUIT_PROCESSOR_H
