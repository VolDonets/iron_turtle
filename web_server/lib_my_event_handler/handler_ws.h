//
// Created by biba_bo on 2020-08-18.
//

#ifndef IRON_TURTLE_HANDLER_WS_H
#define IRON_TURTLE_HANDLER_WS_H

#include "event_ws.h"

/*These is abstract class HandlerWS, which easy to use, when you just extend it
 * it's used for handling events*/
class HandlerWS {
public:
    /*function for handling events, it get a param Event
     * and to do smt when event happens
     * ~> public function*/
    virtual void handleEventWS(EventWS& event) {};

protected:
    HandlerWS() {}
};

#endif //IRON_TURTLE_HANDLER_WS_H
