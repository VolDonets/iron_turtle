//
// Created by biba_bo on 2020-08-18.
//

#ifndef IRON_TURTLE_WEB_SERVER_H
#define IRON_TURTLE_WEB_SERVER_H

#include "seasocks/PrintfLogger.h"
#include "seasocks/Server.h"
#include "seasocks/StringUtil.h"
#include "seasocks/WebSocket.h"
#include "seasocks/util/Json.h"
#include "seasocks/PageHandler.h"
#include "lib_my_event_handler/event_ws.h"
#include "lib_my_event_handler/delegate_ws.h"

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

    virtual void onConnect(WebSocket* connection);
    virtual void onData(WebSocket* connection, const char* data);
    virtual void onDisconnect(WebSocket* connection);

    void sendValuesJSON(std::string values);
    //int getCountConnections();


private:
    set<WebSocket*> _connections;   //set of WebSocket connections
    Server* _server;                //pointer on object of this server
    std::mutex _mutex;              //still useless variable
    DelegateWS* _delegate;          //instance of event delegator

    //EventWS* _eventChangeMQTT;      //instances of possible event - event of enabling or disabling MQTT
    //EventWS* _eventGetMQTT;         //instances of possible event - event of getting MQTT status
    //EventWS* _eventNewClient;       //instances of possible event - adding a new client
    //EventWS* _eventLoseClient;      //instances of possible event - losing an old client
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
