#include "../common/Graph.h"
#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <csignal>

class SimpleEulerServer {
private:
    int port;
    std::atomic<bool> running;
    int serverSocket;
    std::thread serverThread;
    std::mutex logMutex;
    int clientCounter;
    
public:
    SimpleEulerServer(int p) : port(p), running(false), serverSocket(-1), clientCounter(0) {}
    
    ~SimpleEulerServer() {
        stop();
    }
    
    void start() {
        // Create socket
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            throw std::runtime_error("Failed to create socket");
        }
        
        // Allow reuse of address
        int opt = 1;
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        
        // Setup server address
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);
        
        // Bind and listen
        if (bind(serverSocket, (struct sockaddr*)&address, sizeof(address)) < 0) {
            close(serverSocket);
            throw std::runtime_error("Failed to bind to port " + std::to_string(port));
        }
        
        if (listen(serverSocket, 5) < 0) {
            close(serverSocket);
            throw std::runtime_error("Failed to listen on socket");
        }
        
        running = true;
        log("=== Euler Server Started ===");
        log("Port: " + std::to_string(port));
        log("Waiting for connections...");
        
        // Start accepting connections
        serverThread = std::thread(&SimpleEulerServer::acceptLoop, this);
    }
    
    void stop() {
        if (!running) return;
        
        running = false;
        if (serverSocket >= 0) {
            close(serverSocket);
            serverSocket = -1;
        }
        if (serverThread.joinable()) {
            serverThread.join();
        }
        log("=== Server Stopped ===");
    }
    
    bool isRunning() const {
        return running;
    }
    
private:
    void acceptLoop() {
        while (running) {
            struct sockaddr_in clientAddr;
            socklen_t clientLen = sizeof(clientAddr);
            
            int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
            if (clientSocket < 0) {
                if (running) {
                    log("Failed to accept client");
                }
                continue;
            }
            
            int clientId = ++clientCounter;
            std::string clientIP = inet_ntoa(clientAddr.sin_addr);
            log("Client " + std::to_string(clientId) + " connected from " + clientIP);
            
            // Handle client in separate thread
            std::thread clientThread(&SimpleEulerServer::handleClient, this, clientSocket, clientId);
            clientThread.detach();
        }
    }
    
    void handleClient(int clientSocket, int clientId) {
        try {
            // Send welcome message
            std::string welcome = 
                "=== Euler Circuit Server ===\n"
                "Client ID: " + std::to_string(clientId) + "\n"
                "Format: vertices edges edge1 edge2 ...\n"
                "Edge format: u-v (example: 0-1)\n"
                "\n"
                "Examples:\n"
                "  Triangle: 3 3 0-1 1-2 2-0\n"
                "  Square:   4 4 0-1 1-2 2-3 3-0\n"
                "  Path:     3 2 0-1 1-2\n"
                "\n"
                "Commands: help, quit\n"
                "> ";
            
            send(clientSocket, welcome.c_str(), welcome.length(), 0);
            
            char buffer[1024];
            while (running) {
                memset(buffer, 0, sizeof(buffer));
                int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                
                if (bytesRead <= 0) {
                    log("Client " + std::to_string(clientId) + " disconnected");
                    break;
                }
                
                std::string input(buffer);
                input.erase(input.find_last_not_of("\r\n") + 1); // Remove newlines
                
                log("Client " + std::to_string(clientId) + ": " + input);
                
                std::string response;
                
                if (input == "quit" || input == "exit") {
                    response = "Goodbye!\n";
                    send(clientSocket, response.c_str(), response.length(), 0);
                    break;
                } else if (input == "help") {
                    response = welcome;
                } else if (input.empty()) {
                    response = "Empty input. Type 'help' for usage.\n> ";
                } else {
                    // Process graph
                    response = processGraph(input, clientId) + "\n> ";
                }
                
                send(clientSocket, response.c_str(), response.length(), 0);
            }
            
        } catch (const std::exception& e) {
            log("Client " + std::to_string(clientId) + " error: " + std::string(e.what()));
        }
        
        close(clientSocket);
    }
    
    std::string processGraph(const std::string& input, int clientId) {
        try {
            auto start = std::chrono::high_resolution_clock::now();
            
            // Parse graph
            Graph graph = parseGraph(input);
            
            // Analyze Euler circuit
            bool hasEuler = graph.hasEulerCircuit();
            
            std::ostringstream result;
            result << "\n=== ANALYSIS RESULT ===\n";
            result << "Client: " << clientId << "\n";
            result << "Vertices: " << graph.getVertexCount() << "\n";
            result << "Edges: " << graph.getEdgeCount() << "\n";
            result << "Connected: " << (graph.isConnected() ? "Yes" : "No") << "\n";
            
            if (hasEuler) {
                result << "Euler Circuit: ✓ EXISTS\n";
                
                Graph workingGraph = graph;
                auto circuit = workingGraph.findEulerCircuit();
                
                if (!circuit.empty()) {
                    result << "Circuit: ";
                    for (size_t i = 0; i < circuit.size(); ++i) {
                        result << circuit[i];
                        if (i < circuit.size() - 1) result << " → ";
                    }
                    result << "\n";
                    result << "Length: " << circuit.size() << " vertices\n";
                }
            } else {
                result << "Euler Circuit: ✗ NONE\n";
                result << "Reason: ";
                if (!graph.isConnected()) {
                    result << "Graph not connected\n";
                } else {
                    result << "Not all vertices have even degree\n";
                    result << "Degrees: ";
                    for (int i = 0; i < graph.getVertexCount(); ++i) {
                        result << i << "(" << graph.getDegree(i) << ") ";
                    }
                    result << "\n";
                }
            }
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            result << "Processing time: " << duration.count() << "ms\n";
            result << "=======================";
            
            return result.str();
            
        } catch (const std::exception& e) {
            return "\n=== ERROR ===\n" + std::string(e.what()) + "\n=============";
        }
    }
    
    Graph parseGraph(const std::string& input) {
        std::istringstream iss(input);
        int vertices, edges;
        
        if (!(iss >> vertices)) {
            throw std::invalid_argument("Missing or invalid vertices count");
        }
        if (!(iss >> edges)) {
            throw std::invalid_argument("Missing or invalid edges count");
        }
        
        // Validate
        if (vertices <= 0 || vertices > 100) {
            throw std::invalid_argument("Vertices must be 1-100, got: " + std::to_string(vertices));
        }
        if (edges < 0 || edges > vertices * (vertices - 1) / 2) {
            throw std::invalid_argument("Invalid edges count: " + std::to_string(edges));
        }
        
        Graph graph(vertices, false);
        
        // Parse edges
        std::string edgeStr;
        int edgeCount = 0;
        
        while (iss >> edgeStr && edgeCount < edges) {
            size_t dashPos = edgeStr.find('-');
            if (dashPos == std::string::npos) {
                throw std::invalid_argument("Invalid edge format: " + edgeStr + " (expected: u-v)");
            }
            
            try {
                int u = std::stoi(edgeStr.substr(0, dashPos));
                int v = std::stoi(edgeStr.substr(dashPos + 1));
                
                if (u < 0 || u >= vertices || v < 0 || v >= vertices) {
                    throw std::invalid_argument("Vertex out of range: " + edgeStr);
                }
                if (u == v) {
                    throw std::invalid_argument("Self-loops not allowed: " + edgeStr);
                }
                
                graph.addEdge(u, v);
                edgeCount++;
                
            } catch (const std::invalid_argument& e) {
                throw;
            } catch (const std::exception& e) {
                throw std::invalid_argument("Invalid edge: " + edgeStr);
            }
        }
        
        if (edgeCount != edges) {
            throw std::invalid_argument("Expected " + std::to_string(edges) + 
                                      " edges, got " + std::to_string(edgeCount));
        }
        
        return graph;
    }
    
    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(logMutex);
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto tm = *std::localtime(&time_t);
        
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &tm);
        
        std::cout << "[" << timeStr << "] " << message << std::endl;
    }
};

// Global server for signal handling
SimpleEulerServer* globalServer = nullptr;

void signalHandler(int signal) {
    std::cout << "\nReceived signal " << signal << ". Shutting down..." << std::endl;
    if (globalServer) {
        globalServer->stop();
    }
}

int main(int argc, char* argv[]) {
    int port = 8080;
    
    // Parse arguments
    if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " [port]" << std::endl;
        return 1;
    }
    
    if (argc == 2) {
        try {
            port = std::stoi(argv[1]);
            if (port < 1024 || port > 65535) {
                std::cerr << "Port must be 1024-65535" << std::endl;
                return 1;
            }
        } catch (const std::exception& e) {
            std::cerr << "Invalid port: " << argv[1] << std::endl;
            return 1;
        }
    }
    
    try {
        // Setup signal handling
        signal(SIGINT, signalHandler);
        signal(SIGTERM, signalHandler);
        
        SimpleEulerServer server(port);
        globalServer = &server;
        
        std::cout << "=== Q6: Euler Circuit Server ===" << std::endl;
        std::cout << "Starting on port " << port << "..." << std::endl;
        
        server.start();
        
        std::cout << "Server running! Connect with:" << std::endl;
        std::cout << "  telnet localhost " << port << std::endl;
        std::cout << "  or use: ../bin/q6_client" << std::endl;
        std::cout << "Press Ctrl+C to stop." << std::endl;
        
        // Keep running
        while (server.isRunning()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
}