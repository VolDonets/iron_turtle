//
// Created by biba_bo on 2020-08-20.
//

#ifndef IRON_TURTLE_ASCII_COM_LIM_H
#define IRON_TURTLE_ASCII_COM_LIM_H

const std::string DRIVE_FILE_PATH =               "/dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0";

#define MOVING_STEP_1                             1
#define MOVING_STEP_5                             5
#define MOVING_STEP_10                            10

const std::string MOVE_FORWARD =                  "W";
const std::string MOVE_FORWARD_5 =                "WWWWW";
const std::string MOVE_FORWARD_10 =               "WWWWWWWWWW";

const std::string MOVE_BACK =                     "S";
const std::string MOVE_BACK_5 =                   "SSSSS";
const std::string MOVE_BACK_10 =                  "SSSSSSSSSS";

const std::string MOVE_RIGHT =                    "D";
const std::string MOVE_RIGHT_5 =                  "DDDDD";
const std::string MOVE_RIGHT_10 =                 "DDDDDDDDDD";

const std::string MOVE_LEFT =                     "A";
const std::string MOVE_LEFT_5 =                   "AAAAA";
const std::string MOVE_LEFT_10 =                  "AAAAAAAAAA";

const std::string IMMEDIATE_MOVE_STOP =           "X";
const std::string LEAVE_IMMEDIATE_MODE =          "Q";

const std::string IMMEDIATE_MOVING_MODE_SPEED =   "Is\n";
const std::string IMMEDIATE_MOVING_MODE_POWER =   "Iw\n";
const std::string IMMEDIATE_MOVING_MODE_POSN =    "Ip\n";

const std::string GET_BATTERY_INFO =              "C\n";

const std::string EMPTY_COMMAND =                 "a\n";

#define MAX_FORWARD_SPEED                         500
#define MAX_BACKWARD_SPEED                        -500
#define MAX_DIFF_ROT                              100
#define SPEED_STEP                                5
#define ROTATION_STEP                             5

const std::string ACTIVATE_ASCII_MODE =           "unlockASCII\n";

//my own error codes
#define SERIAL_DEV_ACTIVATED                       0
#define SERIAL_DEV_NOT_CONNECTED                   -1
#define SERIAL_DEV_PERMISSION_ERR                  -2
#define SERIAL_DEV_OTHER_ERR                       -3
#define SERIAL_DEV_CONFIG_ERR                      -4
#define SUCCESSFUL_OP                              0
#define UNSUCCESSFUL_OP                            -5

#endif //IRON_TURTLE_ASCII_COM_LIM_H
