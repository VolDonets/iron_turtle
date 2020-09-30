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

    this->eventCamZM = std::make_shared<EventWS>(EVENT_CAM_ZM);
    this->eventCamZP = std::make_shared<EventWS>(EVENT_CAM_ZP);

    this->eventCamUp = std::make_shared<EventWS>(EVENT_CAM_UP);
    this->eventCamDown = std::make_shared<EventWS>(EVENT_CAM_DOWN);
    this->eventCamRight = std::make_shared<EventWS>(EVENT_CAM_RIGHT);
    this->eventCamLeft = std::make_shared<EventWS>(EVENT_CAM_LEFT);

    this->eventClientConnected = std::make_shared<EventWS>(EVENT_CLIENT_CONNECTED);
    this->eventClientDisconnected = std::make_shared<EventWS>(EVENT_CLIENT_DISCONNECTED);

    _isFirstWebRTC_Connection = true;
}

void MyHandler::onConnect(WebSocket* connection) {
    if(_connections.size() == 0) {
        _connections.insert(connection);
        if (_isFirstWebRTC_Connection) {
            _rearSightThread = std::thread([this, connection]() {
                init_rear_sight_processor();
                webrtc_gst_loop(connection);
            });
            _isFirstWebRTC_Connection = false;
        } else {
            webrtc_pipeline_restart(connection);
        }
        _delegate->doEvent(eventClientConnected);
    } else {
        connection->send(MESSAGE_FOR_EXCESS_CLIENT);
        connection->close();
    }
}

void MyHandler::onData(WebSocket* connection, const char* data) {
    if (strstr(data, "comm")) {
        JsonParser *jsonParser = json_parser_new();
        JsonNode *jsonRoot;
        JsonObject *jsonRootObject;
        JsonArray *jsonCommArray;
        if (json_parser_load_from_data(jsonParser, data, -1, NULL)) {
            jsonRoot = json_parser_get_root(jsonParser);
            if (JSON_NODE_HOLDS_OBJECT(jsonRoot)) {
                jsonRootObject = json_node_get_object(jsonRoot);
                jsonCommArray = json_object_get_array_member(jsonRootObject, "comm");
                for (int inx = 0; inx < json_array_get_length(jsonCommArray); inx++) {
                    doEventHandling(json_array_get_string_element(jsonCommArray, inx));
                }
            }
        }
        g_object_unref(jsonParser);
        return;
    }

    webrtc_session_handle(data);
}

void MyHandler::onDisconnect(WebSocket* connection) {
#ifdef UBUNTU_PC
    if (_connections.contains(connection)) {
        _delegate->doEvent(eventClientDisconnected);
    }
#endif //UBUNTU_PC
#ifdef RASPBERRY_PI
    for (WebSocket* c: _connections) {
        if (c == connection) {
            _delegate->doEvent(eventClientDisconnect);
        }
    }
#endif //RASPBERRY_PI

    webrtc_pipeline_deactivate(connection);
    _connections.erase(connection);
}

void MyHandler::sendValuesJSON(std::string values) {
    for (auto c : _connections) {
        c->send(values);
    }
}

void MyHandler::doEventHandling(const char *data) {
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
