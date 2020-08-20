//
// Created by biba_bo on 2020-08-18.
//

#include "web_server.h"
MyHandler::MyHandler(MyServer* server) : _server(server){
    this->_delegate = DelegateWS::getInstance();
}

void MyHandler::onConnect(WebSocket* connection) {
    _connections.insert(connection);
}

void MyHandler::onData(WebSocket* connection, const char* data) {
    cout << "onData: " << data << endl;
}

void MyHandler::onDisconnect(WebSocket* connection) {
    _connections.erase(connection);
}

void MyHandler::sendValuesJSON(std::string values) {
    for (auto c : _connections) {
        c->send(values);
    }
}