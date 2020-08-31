#include <iostream>

/// Included a source code for working with Seasocks server.
#include "web_server_worker.h"

/** This is a start point of the program*/
int main() {
    //this is an instance of the interrupt handling delegator.
    DelegateWS* delegate = DelegateWS::getInstance();
    //this is start point of working webserver and system at all, also here starts threads for webserver, handling thread,
    // and thread for the iron turtle driver.
    WebServerWorker* worker = new WebServerWorker();
    // adding our WebServerWorker as delegator for the WebServer.
    delegate->addHandler(worker);
    // now this line are useful cause, this thread hasn't a loop, so it will fails without joining the server thread
    worker->joinServerTread();
    return 0;
}
