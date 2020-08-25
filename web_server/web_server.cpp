//
// Created by biba_bo on 2020-08-18.
//

#include "../process_camera/rear_sight_webrtc_manipulation.h"

#include "web_server.h"
MyHandler::MyHandler(MyServer* server) : _server(server){
    this->_delegate = DelegateWS::getInstance();

    this->eventMoveForward = std::make_shared<EventWS>(EVENT_MOVE_FORWARD);
    this->eventMoveBack = std::make_shared<EventWS>(EVENT_MOVE_BACK);
    this->eventMoveRighter = std::make_shared<EventWS>(EVENT_MOVE_RIGHTER);
    this->eventMoveLefter = std::make_shared<EventWS>(EVENT_MOVE_LEFTER);

    this->eventCamZM = std::make_shared<EventWS>(EVENT_CAM_ZM);
    this->eventCamZP = std::make_shared<EventWS>(EVENT_CAM_ZP);

    this->eventCamUp = std::make_shared<EventWS>(EVENT_CAM_UP);
    this->eventCamDown = std::make_shared<EventWS>(EVENT_CAM_DOWN);
    this->eventCamRight = std::make_shared<EventWS>(EVENT_CAM_RIGHT);
    this->eventCamLeft = std::make_shared<EventWS>(EVENT_CAM_LEFT);
}

void MyHandler::onConnect(WebSocket* connection) {
    if(_connections.size() == 0) {
        _connections.insert(connection);
        _isCameraStreamEnabled = false;
        _rearSightThread = std::thread([this, connection](){
            std::cout << "OK_4_______________________________________________________________________________________\n";
            webrtc_gst_loop(connection);
            std::cout << "OK_5_______________________________________________________________________________________\n";
        });
    } else {
        connection->send(MESSAGE_FOR_EXCESS_CLIENT);
        connection->close();
    }
}

void MyHandler::onData(WebSocket* connection, const char* data) {
    cout << "onData: " << data << endl;
    if (!_isCameraStreamEnabled) {
        _isCameraStreamEnabled = webrtc_session_handle(data);
    }
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

    if (strcmp(data, COMMAND_CAM_ZM) == 0) {
        _delegate->doEvent(eventCamZM);
        return;
    }
    if (strcmp(data, COMMAND_CAM_ZP) == 0) {
        _delegate->doEvent(eventCamZP);
        return;
    }

    if (strcmp(data, COMMAND_CAM_UP) == 0) {
        _delegate->doEvent(eventCamUp);
        return;
    }
    if (strcmp(data, COMMAND_CAM_DOWN) == 0) {
        _delegate->doEvent(eventCamDown);
        return;
    }
    if (strcmp(data, COMMAND_CAM_RIGHT) == 0) {
        _delegate->doEvent(eventCamRight);
        return;
    }
    if (strcmp(data, COMMAND_CAM_LEFT) == 0) {
        _delegate->doEvent(eventCamLeft);
        return;
    }
}

void MyHandler::onDisconnect(WebSocket* connection) {
    webrtc_session_quit();
    _connections.erase(connection);
}

void MyHandler::sendValuesJSON(std::string values) {
    for (auto c : _connections) {
        c->send(values);
    }
}
