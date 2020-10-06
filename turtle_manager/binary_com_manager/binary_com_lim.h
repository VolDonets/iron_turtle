//
// Created by biba_bo on 2020-09-29.
//

#ifndef IRON_TURTLE_BINARY_COM_LIM_H
#define IRON_TURTLE_BINARY_COM_LIM_H

/// this is a path to the serial device driver
const std::string DRIVE_FILE_PATH = "/dev/ttyUSB0";

/// how my time should wait a thread before send a next command
constexpr int SLEEP_THREAD_TIME_MS = 30000;

/// Cause we use a speed control, and speed controls in percents so max speed defined as float value
constexpr double MAX_SPEED_PERCENT = 1.0;
/// Cause we use a speed control, and speed controls in percents so mim speed defined as float value
/// this value received experimentally
constexpr double MIN_SPEED_PERCENT = 0.01;
/// Cause we want smoothly change a current speed, so we need in a small speed changing
constexpr double SPEED_CHANGE_STEP_PERCENT = 0.01;
/// Speed will change each *?* steps
constexpr int SPEED_CHANGE_TIME_OUT = 1;
/// Cause we want to stop immediately our iron turtle it's so huge
constexpr double SPEED_STOP_STEP_PERCENT = 0.2;
/// Speed will stop each *?* steps
constexpr int SPEED_STOP_TIME_OUT = 5;
/// Maximum power value for the iron turtle engines controller (in pwr points)
constexpr double MAX_WHEELS_POWER_VALUE = 200;
/// Minimum start speed for the iron turtle engine controller (in mm/s)
constexpr int MIN_WHEELS_START_SPEED_VALUE = 5;



/// my own error code -> SERIAL DEVICE ACTIVATED SUCCESSFULLY
constexpr int SERIAL_DEV_ACTIVATED = 0;
/// my own error code -> SERIAL DEVICE IS NOT CONNECTED
constexpr int SERIAL_DEV_NOT_CONNECTED = -1;
/// my own error code -> CANNOT TO USE SERIAL DEVICE DON'T HAVE PERMISSION
constexpr int SERIAL_DEV_PERMISSION_ERR = -2;
/// my own error code -> OTHER TYPE OF ERROR
constexpr int SERIAL_DEV_OTHER_ERR = -3;
/// my own error code -> CONFIGURATION ERROR
constexpr int SERIAL_DEV_CONFIG_ERR = -4;
/// my own error code -> OPERATION DONE WITHOUT ERRORS
constexpr int SUCCESSFUL_OP = 0;
/// my own error code -> OPERATION DONE WITH ERROR
constexpr int UNSUCCESSFUL_OP = -5;

#endif //IRON_TURTLE_BINARY_COM_LIM_H
