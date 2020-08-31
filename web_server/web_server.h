//
// Created by biba_bo on 2020-08-18.
//

#ifndef IRON_TURTLE_WEB_SERVER_H
#define IRON_TURTLE_WEB_SERVER_H

#include <seasocks/PrintfLogger.h>
#include <seasocks/Server.h>
#include <seasocks/StringUtil.h>
#include <seasocks/WebSocket.h>
#include <seasocks/util/Json.h>
#include <seasocks/PageHandler.h>
#include <json-glib/json-glib.h>

#include "lib_my_event_handler/event_ws.h"
#include "lib_my_event_handler/delegate_ws.h"
#include "../process_camera/rear_sight_webrtc_manipulation.h"

#include <iostream>
#include <cstdio>
#include <chrono>
#include <cstdlib>
#include <getopt.h>
#include <memory>
#include <cstring>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <mutex>

#define EVENT_MOVE_FORWARD          1
#define EVENT_MOVE_BACK             2
#define EVENT_MOVE_RIGHTER          3
#define EVENT_MOVE_LEFTER           4

#define COMMAND_MOVE_FORWARD        "MTU"
#define COMMAND_MOVE_BACK           "MTD"
#define COMMAND_MOVE_RIGHTER        "MTR"
#define COMMAND_MOVE_LEFTER         "MTL"

#define EVENT_CAM_ZP                5
#define EVENT_CAM_ZM                6
#define EVENT_CAM_UP                7
#define EVENT_CAM_DOWN              8
#define EVENT_CAM_RIGHT             9
#define EVENT_CAM_LEFT              10

#define COMMAND_CAM_ZP              "ZCP"
#define COMMAND_CAM_ZM              "ZCM"
#define COMMAND_CAM_UP              "MCU"
#define COMMAND_CAM_DOWN            "MCD"
#define COMMAND_CAM_RIGHT           "MCR"
#define COMMAND_CAM_LEFT            "MCL"

#define MESSAGE_FOR_EXCESS_CLIENT   "YOU_ARE_EXCESS"

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

    std::shared_ptr<EventWS> eventMoveForward;
    std::shared_ptr<EventWS> eventMoveBack;
    std::shared_ptr<EventWS> eventMoveRighter;
    std::shared_ptr<EventWS> eventMoveLefter;

    std::shared_ptr<EventWS> eventCamZP;
    std::shared_ptr<EventWS> eventCamZM;
    std::shared_ptr<EventWS> eventCamUp;
    std::shared_ptr<EventWS> eventCamDown;
    std::shared_ptr<EventWS> eventCamLeft;
    std::shared_ptr<EventWS> eventCamRight;

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
