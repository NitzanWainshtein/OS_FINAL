#include "EulerServer.h"
#include <iostream>
#include <csignal>

EulerServer* server = nullptr;

void signalHandler(int sig) {
    std::cout << "\nShutting down server..." << std::endl;
    if (server) {
        server->stop();
    }
}

int main() {
    signal(SIGINT, signalHandler);
    
    server = new EulerServer(8080);
    server->startTcpServer();
    
    std::cout << "\nPress Enter to stop server..." << std::endl;
    std::cin.get();
    
    server->stop();
    delete server;
    return 0;
}