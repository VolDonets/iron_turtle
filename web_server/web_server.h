//
// Created by biba_bo on 2020-08-18.
//

#ifndef IRON_TURTLE_WEB_SERVER_H
#define IRON_TURTLE_WEB_SERVER_H

#include <seasocks/PrintfLogger.h>      // This class used for logging any events with the webserver
#include <seasocks/Server.h>            // This class is just server model
#include <seasocks/StringUtil.h>        // This class contains string utils for an message editing
#include <seasocks/WebSocket.h>         // This class contains class WebSocket which I extends for my own funcionality
#include <seasocks/util/Json.h>         // This class contains utils for working with JSON messages (e.g. the JSON parser)
#include <seasocks/PageHandler.h>       // This class contains a page handler for connection processing
#include <json-glib/json-glib.h>        // This class contains a json parser, which actually uses for the client message parsing.

#include "lib_my_event_handler/event_ws.h"                      // This is a class with a custom event class.
#include "lib_my_event_handler/delegate_ws.h"                   // This is a class with a custom event delegator (works as singleton)
#include "../process_camera/rear_sight_webrtc_manipulation.h"   // This is a .h file with manipulations with current frame and its modification, here uses for activating and other events processing

#include <iostream>    // For the working with external files (as .html, .js, .css, and so on).
#include <cstdio>      //
#include <chrono>
#include <cstdlib>
#include <getopt.h>
#include <memory>      // For using smart pointers
#include <cstring>     // For c-like string comparing
#include <set>         // As a set of the current connections
#include <sstream>
#include <string>
#include <thread>      // An object of the created threads
#include <mutex>       // For thread locking (possybly will be usefull)

/// My own event code (this means a client is wanting to move the iron turtle in the FORWARD direction faster).
constexpr int EVENT_MOVE_FORWARD = 1;
/// My own event code (this means a client is wanting to move the iron turtle in the BACKWARD direction faster).
constexpr int EVENT_MOVE_BACK = 2;
/// My own event code (this means a client is wanting to move the iron turtle in the RIGHT direction).
constexpr int EVENT_MOVE_RIGHTER = 3;
/// My own event code (this means a client is wanting to move the iron turtle in the LEFT direction).
constexpr int EVENT_MOVE_LEFTER = 4;
/// My own event code (this means a client is wanting to STOP the iron turtle moving).
constexpr int EVENT_STOP_MOVING = 13;

/// This command gets from a client, if the client wants to move in the FORWARD direction.
#define COMMAND_MOVE_FORWARD        "MTU"
/// This command gets from a client, if the client wants to move in the BACKWARD direction.
#define COMMAND_MOVE_BACK           "MTD"
/// This command gets from a client, if the client wants to move in the RIGHT direction.
#define COMMAND_MOVE_RIGHTER        "MTR"
/// This command gets from a client, if the client wants to move in the LEFT direction.
#define COMMAND_MOVE_LEFTER         "MTL"
/// This command gets from a client, if the client wants to stop the iron turtle moving
#define COMMAND_STOP_MOVING         "MTS"

/// My own event code (this means a client is wanting to decrease ROI's size)
constexpr int EVENT_CAM_ZP = 5;
/// My own event code (this means a client is wanting to increase ROI's size)
constexpr int EVENT_CAM_ZM = 6;
/// My own event code (this means a client is wanting to move the ROI's rectangle UP)
constexpr int EVENT_CAM_UP = 7;
/// My own event code (this means a client is wanting to move the ROI's rectangle DOWN)
constexpr int EVENT_CAM_DOWN = 8;
/// My own event code (this means a client is wanting to move the ROI's rectangle RIGHT)
constexpr int EVENT_CAM_RIGHT = 9;
/// My own event code (this means a client is wanting to move the ROI's rectangle LEFT)
constexpr int EVENT_CAM_LEFT = 10;

/// This command gets from a client, if the client wants to decrease the ROI's rectangle)
#define COMMAND_CAM_ZP              "ZCP"
/// This command gets from a client, if the client wants to increase the ROI's rectangle)
#define COMMAND_CAM_ZM              "ZCM"
/// This command gets from a client, if the client wants to move the ROI's rectangle UP)
#define COMMAND_CAM_UP              "MCU"
/// This command gets from a client, if the client wants to move the ROI's rectangle DOWN)
#define COMMAND_CAM_DOWN            "MCD"
/// This command gets from a client, if the client wants to move the ROI's rectangle RIGHT)
#define COMMAND_CAM_RIGHT           "MCR"
/// This command gets from a client, if the client wants to move the ROI's rectangle LEFT)
#define COMMAND_CAM_LEFT            "MCL"

/// My own event code (this means we have a new active client's connection)
constexpr int EVENT_CLIENT_CONNECTED = 11;
/// My own event code (this means we loose an active client's connection)
constexpr int EVENT_CLIENT_DISCONNECTED = 12;

/// This command gets from a client, if the client wants
#define COMMAND_CLIENT_IS_STILL_HERE    "OK"

/// This command gets from a client, if the client wants
#define MESSAGE_FOR_EXCESS_CLIENT   "YOU_ARE_EXCESS"

#ifdef MY_PURSUIT_TESTING
/// this is a code for the event on increasing the pursuit rectangle
constexpr int EVENT_PURSUIT_ZOOM_PLUS = 1013;
/// this is a code for the event on decreasing the pursuit rectangle
constexpr int EVENT_PURSUIT_ZOOM_MINUS = 1014;
/// this is a code for the event on moving the pursuit rectangle lefter
constexpr int EVENT_PURSUIT_LEFTER = 1015;
/// this is a code for the event on moving the pursuit rectangle righter
constexpr int EVENT_PURSUIT_RIGHTER = 1016;

/// this command the server gets from a client, when the client wants to increase size of the pursuit rectangle
#define COMMAND_PURSUIT_ZOOM_PLUS       "ZPP"
/// this command the server gets from a client, when the client wants to decrease size of the pursuit rectangle
#define COMMAND_PURSUIT_ZOOM_MINUS      "ZMP"
/// this command the server gets from a client, when the client wants to move righter the pursuit rectangle
#define COMMAND_PURSUIT_MOVE_RIGHTER    "MRP"
/// this command the server gets from a client, when the client wants to move lefter the pursuit rectangle
#define COMMAND_PURSUIT_MOVE_LEFTER     "MLP"
#endif //MY_PURSUIT_TESTING

using namespace std::chrono;
using namespace seasocks;
using namespace std;

class MyServer : public Server {
public:
    using Server::Server;

private:
    virtual void checkThread() const override {

    };
};


class MyHandler : public WebSocket::Handler {
public:
    explicit MyHandler(MyServer* server);

    /// overloading a handling function onConnect
    virtual void onConnect(WebSocket* connection);
    /// overloading a handling function onMessage or onData
    virtual void onData(WebSocket* connection, const char* data);
    /// overloading a handlilg function onDisconnect
    virtual void onDisconnect(WebSocket* connection);

    /// a function for sending messages for clients (in this version of the server for one client)
    void sendValuesJSON(std::string values);


private:
    ///set of WebSocket connections
    set<WebSocket*> _connections;
    ///pointer on object of this server
    Server* _server;
    ///instance of event delegator
    DelegateWS* _delegate;
    ///thread for new connection
    std::thread _rearSightThread;

    /** this variable answers on the question "Had it WebRTC connection/gst loop?"
      also by this variable we decide to create a new thread for gst loop or not*/
    bool _isFirstWebRTC_Connection;

    /// smart pointer to my own event "moving the iron turtle forward"
    std::shared_ptr<EventWS> eventMoveForward;
    /// smart pointer to my own event "moving the iron turtle backward"
    std::shared_ptr<EventWS> eventMoveBack;
    /// smart pointer to my own event "moving the iron turtle righter"
    std::shared_ptr<EventWS> eventMoveRighter;
    /// smart pointer to my own event "moving the iron turtle lefter"
    std::shared_ptr<EventWS> eventMoveLefter;
    /// smart pointer to my own event "stopping the iron turtle moving"
    std::shared_ptr<EventWS> eventStopMoving;

    /// smart pointer to my own event "decreasing the ROI's rectangle size"
    std::shared_ptr<EventWS> eventCamZP;
    /// smart pointer to my own event "increasing the ROI's rectangle size"
    std::shared_ptr<EventWS> eventCamZM;
    /// smart pointer to my own event "moving the ROI's rectangle up"
    std::shared_ptr<EventWS> eventCamUp;
    /// smart pointer to my own event "moving the ROI's rectangle down"
    std::shared_ptr<EventWS> eventCamDown;
    /// smart pointer to my own event "moving the ROI's rectangle left"
    std::shared_ptr<EventWS> eventCamLeft;
    /// smart pointer to my own event "moving the ROI's rectangle right"
    std::shared_ptr<EventWS> eventCamRight;

    /// smart pointer to my own event "have a new active client's connection"
    std::shared_ptr<EventWS> eventClientConnected;
    /// smart pointer to my own event "lose an active client's connection"
    std::shared_ptr<EventWS> eventClientDisconnected;

#ifdef MY_PURSUIT_TESTING
    ///smart pointer to my own event "increasing the pursuit rectangle"
    std::shared_ptr<EventWS> eventIncreasePursuitRect;
    ///smart pointer to my own event "decreasing the pursuit rectangle"
    std::shared_ptr<EventWS> eventDecreasePursuitRect;
    ///smart pointer to my own event "moving lefter the pursuit rectangle"
    std::shared_ptr<EventWS> eventMovePursuitRectLefter;
    ///smart pointer to my own event "moving righter the pursuit rectangle"
    std::shared_ptr<EventWS> eventMovePursuitRectRighter;
#endif //MY_PURSUIT_TESTING

    /// a private function for handling a vector commands from a client
    void doEventHandling(const char *command);
};

struct MyAuthHandler : PageHandler {
    std::shared_ptr<Response> handle(const Request& request) override {
        // Here one would handle one's authentication system, for example;
        // * check to see if the user has a trusted cookie: if so, accept it.
        // * if not, redirect to a login handler page, and await a redirection
        //   back here with relevant URL parameters indicating success. Then,
        //   set the cookie.
        // For this example, we set the user's authentication information purely
        // from their connection.
        request.credentials()->username = formatAddress(request.getRemoteAddress());
        return Response::unhandled(); // cause next handler to run
    }
};


#endif //IRON_TURTLE_WEB_SERVER_H
