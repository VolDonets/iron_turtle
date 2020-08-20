//
// Created by biba_bo on 2020-08-18.
//

#include "web_server_worker.h"

#include <zconf.h>


WebServerWorker::WebServerWorker() {
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
