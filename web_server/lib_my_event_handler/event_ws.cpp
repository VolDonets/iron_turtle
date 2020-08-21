//
// Created by biba_bo on 2020-08-18.
//

#include "event_ws.h"

EventWS::EventWS(int idEvent) {
    this->idEvent = idEvent;
}

int EventWS::getEventID() {
    return this->idEvent;
}