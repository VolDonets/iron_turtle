//
// Created by biba_bo on 2020-08-18.
//

#ifndef IRON_TURTLE_WEB_SERVER_WORKER_H
#define IRON_TURTLE_WEB_SERVER_WORKER_H

#define MAX_JSON_LIST_SIZE 20                //set a max list's size with data from sensors

#include <random>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "web_server/web_server.h"
#include "web_server/lib_my_event_handler/handler_ws.h"
#include "turtle_manager/ascii_com_manager/ascii_com_lim.h"
#include "turtle_manager/turtle_manager.h"

class WebServerWorker : public HandlerWS{
public:
    WebServerWorker();
    void joinServerTread();
    void handleEventWS(std::shared_ptr<EventWS> event) override;

private:
    void startServer();
    void processServer();

    std::thread server_thread;
    shared_ptr<PrintfLogger> logger;
    shared_ptr<MyServer> ws_server;
    shared_ptr<MyHandler> handler;

    shared_ptr<TurtleManager> turtle_manager;
};


#endif //IRON_TURTLE_WEB_SERVER_WORKER_H