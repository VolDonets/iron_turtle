#include <iostream>

#include "web_server/web_server_worker.h"

int main() {
    std::shared_ptr<WebServerWorker> server_worker = std::make_shared<WebServerWorker>();
    server_worker->joinServerTread();
    return 0;
}
