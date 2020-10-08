//
// Created by biba_bo on 2020-08-18.
//

#include "web_server_worker.h"

#include <zconf.h>


WebServerWorker::WebServerWorker() {
    // creating an object of TurtleManager with connecting throw COM-port
    turtle_manager = make_shared<SmoothTurtleManager>();
    // starting the iron turtle moving processing thread
    turtle_manager->restart_processing_thread();
    // starting web-server
    this->startServer();
    // this thread sleeps for 1 seconds for avoiding bugs and errors
    sleep(1);
    // staring processing the server.
    this->processServer();
}

void WebServerWorker::startServer() {
    // creating a new thread for the web-server
    this->server_thread = std::thread([this]() {
        // creating a new object of PrintfLogger for possibility to log working process
        logger = std::make_shared<PrintfLogger>();
        // creating a new object of MyServer and adding here a logger
        ws_server = make_shared<MyServer>(logger);
        // creating a new object of MyHandler for handling events from web-client (onConnect, onDisconnect, onMessage)
        handler = std::make_shared<MyHandler>(ws_server.get());
        // adding a page handler for working with web-pages
        ws_server->addPageHandler(std::make_shared<MyAuthHandler>());
        // added a socket handler
        ws_server->addWebSocketHandler("/chart", handler);
        // starting webserver
        ws_server->serve("src/server_files", 56778);
    });
}

void WebServerWorker::processServer() {
    // still nothing works here
}

void WebServerWorker::joinServerTread() {
    // like a feature for avoiding bugs
    if (server_thread.joinable())
        server_thread.join();
}

void WebServerWorker::handleEventWS(std::shared_ptr<EventWS> event) {
    // handling events by the event code
    switch(event->getEventID()) {
        case EVENT_MOVE_FORWARD:
            turtle_manager->move_forward();
            break;
        case EVENT_MOVE_BACK:
            turtle_manager->move_backward();
            break;
        case EVENT_MOVE_LEFTER:
            turtle_manager->move_lefter();
            break;
        case EVENT_MOVE_RIGHTER:
            turtle_manager->move_righter();
            break;
        case EVENT_STOP_MOVING:
            turtle_manager->stop_moving();
            break;
        case EVENT_CAM_ZM:
            on_zoom_minus_processor();
            break;
        case EVENT_CAM_ZP:
            on_zoom_plus_processor();
            break;
        case EVENT_CAM_UP:
            on_move_up_processor();
            break;
        case EVENT_CAM_DOWN:
            on_move_down_processor();
            break;
        case EVENT_CAM_LEFT:
            on_move_left_processor();
            break;
        case EVENT_CAM_RIGHT:
            on_move_right_processor();
            break;
        case EVENT_CLIENT_CONNECTED:
            turtle_manager->say_server_here();
            break;
        case EVENT_CLIENT_DISCONNECTED:
            turtle_manager->say_server_leave();
            break;
#ifdef MY_PURSUIT_TESTING
        case EVENT_PURSUIT_ZOOM_PLUS:
            increase_object_rectangle();
            break;
        case EVENT_PURSUIT_ZOOM_MINUS:
            decrease_object_rectangle();
            break;
        case EVENT_PURSUIT_LEFTER:
            move_lefter_object_rectangle();
            break;
        case EVENT_PURSUIT_RIGHTER:
            move_righter_object_rectangle();
            break;
#endif //MY_PURSUIT_TESTING
    }
}
