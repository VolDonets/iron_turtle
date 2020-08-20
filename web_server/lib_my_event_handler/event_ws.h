//
// Created by biba_bo on 2020-08-18.
//

#ifndef IRON_TURTLE_EVENT_WS_H
#define IRON_TURTLE_EVENT_WS_H

/*These is a class EventWS, which used for contains an event state*/
class EventWS {
private:
    /*logical field with information about EVENT*/
    int idEvent;

public:
    /*Constructor for creating a new EventWS object,
     * ~>public constructor*/
    EventWS(int);

    /*function for getting an event information
     * ~>public function*/
    int getEventID();
};

#endif //IRON_TURTLE_EVENT_WS_H