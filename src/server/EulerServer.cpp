#include "EulerServer.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

EulerServer::EulerServer(int serverPort) : port(serverPort), running(false), serverSocket(-1) {
}

EulerServer::~EulerServer() {
    stop();
}

// Old simulation start (for compatibility)
void EulerServer::start() {
    running = true;
    std::cout << "Euler Circle Server started on port " << port << std::endl;
}

void EulerServer::stop() {
    running = false;
    if (serverSocket >= 0) {
        close(serverSocket);
        serverSocket = -1;
    }
    if (serverThread.joinable()) {
        serverThread.join();
    }
    std::cout << "Euler Circle Server stopped" << std::endl;
}

// Old simulation method (for compatibility)
EulerServer::EulerResult EulerServer::processRequest(const GraphRequest& request) {
    EulerResult result(request.clientId);
    
    std::cout << "Processing Euler circle request from client " << request.clientId << std::endl;
    
    Graph workingGraph = request.graph;
    result.hasEulerCircle = workingGraph.hasEulerCircuit();
    
    if (result.hasEulerCircle) {
        result.eulerCircle = workingGraph.findEulerCircuit();
        result.message = "Euler circle found";
    } else {
        result.message = "No Euler circle exists";
    }
    
    std::cout << "Result for client " << request.clientId << ": " << result.message << std::endl;
    return result;
}

// Real TCP Server
void EulerServer::startTcpServer() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }
    
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    if (bind(serverSocket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(serverSocket);
        return;
    }
    
    if (listen(serverSocket, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(serverSocket);
        return;
    }
    
    running = true;
    std::cout << "=== TCP Euler Server Started ===" << std::endl;
    std::cout << "Port: " << port << std::endl;
    std::cout << "Connect with: telnet localhost " << port << std::endl;
    std::cout << "Format: vertices edges edge1 edge2 ..." << std::endl;
    std::cout << "Example: 4 4 0-1 1-2 2-3 3-0" << std::endl;
    std::cout << "===============================" << std::endl;
    
    serverThread = std::thread([this]() {
        while (running) {
            struct sockaddr_in clientAddr;
            socklen_t clientLen = sizeof(clientAddr);
            
            int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
            if (clientSocket < 0) {
                if (running) {
                    std::cerr << "Accept failed" << std::endl;
                }
                continue;
            }
            
            std::cout << "Client connected!" << std::endl;
            handleClient(clientSocket);
        }
    });
}

void EulerServer::handleClient(int clientSocket) {
    // Send welcome message with examples
    std::string welcome = "=== Euler Circle Server ===\n";
    welcome += "Send graph in format: vertices edges edge1 edge2 ...\n";
    welcome += "Edge format: v1-v2 (e.g., 0-1 means edge from vertex 0 to 1)\n\n";
    welcome += "Examples:\n";
    welcome += "  Triangle: 3 3 0-1 1-2 2-0\n";
    welcome += "  Square:   4 4 0-1 1-2 2-3 3-0\n";
    welcome += "  Path:     3 2 0-1 1-2\n";
    welcome += "  K4:       4 6 0-1 0-2 0-3 1-2 1-3 2-3\n\n";
    welcome += "Type 'quit' to exit\n> ";
    
    send(clientSocket, welcome.c_str(), welcome.length(), 0);
    
    char buffer[512];
    while (running) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytesRead <= 0) break;
        
        std::string input(buffer);
        input.erase(input.find_last_not_of("\r\n") + 1);
        
        std::cout << "Received: '" << input << "'" << std::endl;
        
        if (input == "quit" || input == "exit") {
            std::string bye = "Goodbye!\n";
            send(clientSocket, bye.c_str(), bye.length(), 0);
            break;
        }
        
        std::string response = processEulerRequestTcp(input);
        response += "\n> ";
        send(clientSocket, response.c_str(), response.length(), 0);
    }
    
    close(clientSocket);
    std::cout << "Client disconnected" << std::endl;
}

std::string EulerServer::processEulerRequestTcp(const std::string& input) {
    std::istringstream iss(input);
    int vertices, edges;
    
    // Parse vertices and edges count
    if (!(iss >> vertices >> edges)) {
        return "Error: Invalid format. Expected: vertices edges edge1 edge2 ...";
    }
    
    if (vertices <= 0 || vertices > 20) {
        return "Error: Vertices must be 1-20";
    }
    
    if (edges < 0) {
        return "Error: Edges count cannot be negative";
    }
    
    // Create empty graph
    Graph graph(vertices, false); // undirected
    
    // Parse edges
    std::string edgeStr;
    int edgeCount = 0;
    
    while (iss >> edgeStr && edgeCount < edges) {
        size_t dashPos = edgeStr.find('-');
        if (dashPos == std::string::npos) {
            return "Error: Invalid edge format '" + edgeStr + "'. Expected format: v1-v2";
        }
        
        try {
            int v1 = std::stoi(edgeStr.substr(0, dashPos));
            int v2 = std::stoi(edgeStr.substr(dashPos + 1));
            
            if (v1 < 0 || v1 >= vertices || v2 < 0 || v2 >= vertices) {
                return "Error: Vertex " + std::to_string((v1 < 0 || v1 >= vertices) ? v1 : v2) + " out of range (0-" + std::to_string(vertices-1) + ")";
            }
            
            graph.addEdge(v1, v2);
            edgeCount++;
            
        } catch (const std::exception& e) {
            return "Error: Invalid vertex number in edge '" + edgeStr + "'";
        }
    }
    
    if (edgeCount != edges) {
        return "Error: Expected " + std::to_string(edges) + " edges, but got " + std::to_string(edgeCount);
    }
    
    // Display the parsed graph
    std::cout << "Parsed graph with " << vertices << " vertices, " << edges << " edges:" << std::endl;
    graph.printGraph();
    
    // Check for Euler circuit
    std::stringstream response;
    response << "\n=== GRAPH ANALYSIS ===\n";
    response << "Vertices: " << vertices << ", Edges: " << edges << "\n";
    
    if (graph.hasEulerCircuit()) {
        auto circuit = graph.findEulerCircuit();
        response << "✓ EULER CIRCUIT EXISTS!\n";
        response << "Circuit: ";
        for (size_t i = 0; i < circuit.size(); ++i) {
            response << circuit[i];
            if (i < circuit.size() - 1) response << " -> ";
        }
        response << "\n";
        response << "Length: " << circuit.size() << " vertices\n";
    } else {
        response << "✗ NO EULER CIRCUIT\n";
        response << "Reason: Not all vertices have even degree\n";
        
        // Show degrees
        response << "Vertex degrees: ";
        for (int i = 0; i < vertices; ++i) {
            auto neighbors = graph.getNeighbors(i);
            int degree = neighbors.size();
            response << i << "(" << degree << ") ";
        }
        response << "\n";
    }
    
    response << "=====================";
    return response.str();
}