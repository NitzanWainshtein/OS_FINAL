#include "LFServer.h"
#include <iostream>
#include <csignal>

LFServer* globalServer = nullptr;

void signalHandler(int signal) {
    std::cout << "\nReceived signal " << signal << ". Shutting down..." << std::endl;
    if (globalServer) {
        globalServer->stop();
    }
}

void printUsage(const char* progName) {
    std::cout << "=== Q8: Leader-Follower Server ===" << std::endl;
    std::cout << "Usage: " << progName << " [port] [workers]" << std::endl;
    std::cout << "  port:    TCP port (default: 8080)" << std::endl;
    std::cout << "  workers: Number of worker threads (default: 4)" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  " << progName << "          # port 8080, 4 workers" << std::endl;
    std::cout << "  " << progName << " 9000     # port 9000, 4 workers" << std::endl;
    std::cout << "  " << progName << " 8080 8   # port 8080, 8 workers" << std::endl;
}

int main(int argc, char* argv[]) {
    int port = 8080;
    size_t numWorkers = 4;
    
    // Parse arguments
    if (argc > 3) {
        std::cerr << "Too many arguments" << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    if (argc >= 2) {
        std::string arg1 = argv[1];
        if (arg1 == "-h" || arg1 == "--help") {
            printUsage(argv[0]);
            return 0;
        }
        
        try {
            port = std::stoi(arg1);
            if (port < 1024 || port > 65535) {
                std::cerr << "Port must be 1024-65535" << std::endl;
                return 1;
            }
        } catch (const std::exception& e) {
            std::cerr << "Invalid port: " << arg1 << std::endl;
            return 1;
        }
    }
    
    if (argc == 3) {
        try {
            numWorkers = std::stoi(argv[2]);
            if (numWorkers < 1 || numWorkers > 16) {
                std::cerr << "Workers must be 1-16" << std::endl;
                return 1;
            }
        } catch (const std::exception& e) {
            std::cerr << "Invalid workers count: " << argv[2] << std::endl;
            return 1;
        }
    }
    
    try {
        // Setup signal handling
        signal(SIGINT, signalHandler);
        signal(SIGTERM, signalHandler);
        
        LFServer server(port, numWorkers);
        globalServer = &server;
        
        std::cout << "=== Q8: Leader-Follower Algorithms Server ===" << std::endl;
        std::cout << "Port: " << port << std::endl;
        std::cout << "Worker threads: " << numWorkers << std::endl;
        std::cout << "Pattern: Leader-Follower" << std::endl;
        std::cout << "=============================================" << std::endl;
        
        server.start();
        
        std::cout << "\nServer running! Connect with:" << std::endl;
        std::cout << "  telnet localhost " << port << std::endl;
        std::cout << "  or use: ../bin/q8_client" << std::endl;
        std::cout << "\nExecutes ALL algorithms on each graph:" << std::endl;
        std::cout << "  euler, clique, mst, hamilton, scc" << std::endl;
        std::cout << "\nPress Ctrl+C to stop and see statistics." << std::endl;
        
        // Keep running
        while (server.isRunning()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        
        // Print final statistics
        server.printStatistics();
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
}