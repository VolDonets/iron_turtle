//
// Created by biba_bo on 2020-08-18.
//

#include "web_server.h"
MyHandler::MyHandler(MyServer* server) : _server(server){
    this->_delegate = DelegateWS::getInstance();

    this->eventMoveForward = std::make_shared<EventWS>(EVENT_MOVE_FORWARD);
    this->eventMoveBack = std::make_shared<EventWS>(EVENT_MOVE_BACK);
    this->eventMoveRighter = std::make_shared<EventWS>(EVENT_MOVE_RIGHTER);
    this->eventMoveLefter = std::make_shared<EventWS>(EVENT_MOVE_LEFTER);
}

void MyHandler::onConnect(WebSocket* connection) {
    if(_connections.size() == 0)
        _connections.insert(connection);
    else
        connection->close();
}

void MyHandler::onData(WebSocket* connection, const char* data) {
    cout << "onData: " << data << endl;
    if (strcmp(data, COMMAND_MOVE_FORWARD) == 0) {
        _delegate->doEvent(eventMoveForward);
        return;
    }
    if (strcmp(data, COMMAND_MOVE_BACK) == 0) {
        _delegate->doEvent(eventMoveBack);
        return;
    }
    if (strcmp(data, COMMAND_MOVE_RIGHTER) == 0) {
        _delegate->doEvent(eventMoveRighter);
        return;
    }
    if (strcmp(data, COMMAND_MOVE_LEFTER) == 0) {
        _delegate->doEvent(eventMoveLefter);
        return;
    }
}

void MyHandler::onDisconnect(WebSocket* connection) {
    _connections.erase(connection);
}

void MyHandler::sendValuesJSON(std::string values) {
    for (auto c : _connections) {
        c->send(values);
    }
}