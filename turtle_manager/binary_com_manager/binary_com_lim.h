//
// Created by biba_bo on 2020-09-29.
//

#ifndef IRON_TURTLE_BINARY_COM_LIM_H
#define IRON_TURTLE_BINARY_COM_LIM_H

/// this is a path to the serial device driver
#define DRIVE_FILE_PATH                            "/dev/ttyUSB0"

/// how my time should wait a thread before send a next command
#define SLEEP_THREAD_TIME_MS                       40000

/// Cause we use a speed control, and speed controls in percents so max speed defined as float value
#define MAX_SPEED_PERCENT                          1.0
/// Cause we use a speed control, and speed controls in percents so mim speed defined as float value
/// this value received experimentally
#define MIN_SPEED_PERCENT                          0.15
/// Cause we want smoothly change a current speed, so we need in a small speed changing
#define SPEED_CHANGE_STEP_PERCENT                0.05
/// Speed will change each *?* steps
#define SPEED_CHANGE_TIME_OUT                    4
/// Cause we want to stop immediately our iron turtle it's so huge
#define SPEED_STOP_STEP_PERCENT                0.2
/// Speed will stop each *?* steps
#define SPEED_STOP_TIME_OUT                    5




/// my own error code -> SERIAL DEVICE ACTIVATED SUCCESSFULLY
#define SERIAL_DEV_ACTIVATED                       0
/// my own error code -> SERIAL DEVICE IS NOT CONNECTED
#define SERIAL_DEV_NOT_CONNECTED                   -1
/// my own error code -> CANNOT TO USE SERIAL DEVICE DON'T HAVE PERMISSION
#define SERIAL_DEV_PERMISSION_ERR                  -2
/// my own error code -> OTHER TYPE OF ERROR
#define SERIAL_DEV_OTHER_ERR                       -3
/// my own error code -> CONFIGURATION ERROR
#define SERIAL_DEV_CONFIG_ERR                      -4
/// my own error code -> OPERATION DONE WITHOUT ERRORS
#define SUCCESSFUL_OP                              0
/// my own error code -> OPERATION DONE WITH ERROR
#define UNSUCCESSFUL_OP                            -5

#endif //IRON_TURTLE_BINARY_COM_LIM_H
