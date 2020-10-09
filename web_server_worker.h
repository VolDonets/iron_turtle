//
// Created by biba_bo on 2020-08-18.
//

#ifndef IRON_TURTLE_WEB_SERVER_WORKER_H
#define IRON_TURTLE_WEB_SERVER_WORKER_H

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
#include "turtle_manager/smooth_turtle_manager.h"
#include "process_camera/rear_sight_webrtc_manipulation.h"

/** This is the class WebServerWorker, which extends the abstract class HandlerWS (which are a part of my own handling system)
 * it used for handling interrupts from the WebServer, and works width hardware drivers*/
class WebServerWorker : public HandlerWS{
public:
    /** Main constructor here starts a new thread for the webserver*/
    WebServerWorker();
    /** Destructor*/
    ~WebServerWorker();
    /** The function for joining a webserver's thread */
    void joinServerTread();
    /** The overriding a function from the class HandlerWS */
    void handleEventWS(std::shared_ptr<EventWS> event) override;

private:
    /** The private method for starting web-server thread */
    void startServer();
    /** Method for processing a "hard" request from webserver in the another thread */
    void processServer();

    /** The field with the web-server thread */
    std::thread server_thread;
    /** The field with the loggering system for web-server */
    shared_ptr<PrintfLogger> logger;
    /** The field with MySever - this is my web-server, which process connection with web clients */
    shared_ptr<MyServer> ws_server;
    /** The field with MyHandler - this is my hander for events from web-clients, but it handle only events:
     * onConnection, onDisconnecting, onMessage, and only in the web-server thread*/
    shared_ptr<MyHandler> handler;

    /** The field with TurtleManager. This one are manager of the iron turtle driver. It just gets commands from this class
     * and them process it if possible*/
    shared_ptr<SmoothTurtleManager> turtle_manager;

    /** @brief this field contains a smart pointer to the PursuitProcessor class. This one are manager of the iron turtle driver.
     *         it gets an object coordinates from current frame processing class and try to pursuit the aim (here is an detected
     *         object, and them an tracked object).*/
    shared_ptr<PursuitProcessor> pursuit_processor;
};


#endif //IRON_TURTLE_WEB_SERVER_WORKER_H