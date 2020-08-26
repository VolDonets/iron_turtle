//
// Created by biba_bo on 2020-08-18.
//

#include "web_server_worker.h"

#include <zconf.h>


WebServerWorker::WebServerWorker() {
    turtle_manager = make_shared<TurtleManager>(MOVING_STEP_10, MOVING_STEP_5);
    this->startServer();
    sleep(1);
    this->processServer();
}

void WebServerWorker::startServer() {
    this->server_thread = std::thread([this]() {
        logger = std::make_shared<PrintfLogger>();
        ws_server = make_shared<MyServer>(logger);
        handler = std::make_shared<MyHandler>(ws_server.get());
        ws_server->addPageHandler(std::make_shared<MyAuthHandler>());
        ws_server->addWebSocketHandler("/chart", handler);
        ws_server->serve("src/server_files", 56778);
    });
}

void WebServerWorker::processServer() {

}

void WebServerWorker::joinServerTread() {
    if (server_thread.joinable())
        server_thread.join();
}

void WebServerWorker::handleEventWS(std::shared_ptr<EventWS> event) {
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
        case EVENT_CAM_ZM:
            rear_sight_processor->on_zoom_minus_processor();
            rear_sight_processor->set_new_frame_param();
            break;
        case EVENT_CAM_ZP:
            rear_sight_processor->on_zoom_plus_processor();
            rear_sight_processor->set_new_frame_param();
            break;
        case EVENT_CAM_UP:
            rear_sight_processor->on_move_up_processor();
            rear_sight_processor->set_new_frame_param();
            break;
        case EVENT_CAM_DOWN:
            rear_sight_processor->on_move_down_processor();
            rear_sight_processor->set_new_frame_param();
            break;
        case EVENT_CAM_LEFT:
            rear_sight_processor->on_move_left_processor();
            rear_sight_processor->set_new_frame_param();
            break;
        case EVENT_CAM_RIGHT:
            rear_sight_processor->on_move_right_processor();
            rear_sight_processor->set_new_frame_param();
            break;
    }
}
