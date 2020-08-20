//
// Created by biba_bo on 2020-08-18.
//

#ifndef IRON_TURTLE_DELEGATE_WS_H
#define IRON_TURTLE_DELEGATE_WS_H

#include <iostream>
#include <functional>

#include "event_ws.h"
#include "handler_ws.h"

/*Class for delegating Handlers and Events, works like by SINGLETON pattern*/
class DelegateWS {
private:
    static DelegateWS* p_instance;          //private static field for the singular link on the DelegateWS object
    std::vector<HandlerWS*> m_handlers;       //private field with link on list of connected handlers
    //std::list<HandlerWS*> m_handlers;       //private field with link on list of connected handlers

    DelegateWS() {}                         //private constructor
    DelegateWS(const DelegateWS&);
    DelegateWS&operator= (DelegateWS&);     //announced using of '=' operator

public:
    static DelegateWS* getInstance();       //public static function for getting an instance on the singular DelegateWS's object
    void doEvent(EventWS&);                 //public function for doing event //I SHOULD CHANGE IT, BUT I DON'T KNOW HOW
    void addHandler(HandlerWS&);            //public function for adding a new HandlerWS& object
};

#endif //IRON_TURTLE_DELEGATE_WS_H