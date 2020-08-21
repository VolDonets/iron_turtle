#include <iostream>
#include <zconf.h>

#include "web_server_worker.h"


int main() {
    DelegateWS* delegate = DelegateWS::getInstance();
    WebServerWorker* worker = new WebServerWorker();
    delegate->addHandler(worker);
    worker->joinServerTread();
    return 0;
}
