// src/server/main_euler.cpp - Fixed version
#include "EulerServer.h"
#include "../graph/GraphGenerator.h"
#include <iostream>

void testEulerServer() {
    std::cout << "=== Testing Euler Server (Requirement 6) ===" << std::endl;
    
    // Create server
    EulerServer server(8080);
    server.start();
    
    // Test 1: Graph with Euler circuit
    std::cout << "\n--- Test 1: Graph with Euler circuit ---" << std::endl;
    Graph testGraph1(4, false);
    testGraph1.addEdge(0, 1);
    testGraph1.addEdge(1, 2);
    testGraph1.addEdge(2, 3);
    testGraph1.addEdge(3, 0);
    testGraph1.addEdge(0, 2);
    testGraph1.addEdge(1, 3);
    
    GraphRequest request1(1, testGraph1);
    EulerServer::EulerResult result1 = server.processRequest(request1);
    
    std::cout << "Client: " << result1.clientId << std::endl;
    std::cout << "Has Euler Circle: " << (result1.hasEulerCircle ? "Yes" : "No") << std::endl;
    
    if (result1.hasEulerCircle) {
        std::cout << "Euler Circle: ";
        for (size_t i = 0; i < result1.eulerCircle.size(); i++) {
            std::cout << result1.eulerCircle[i];
            if (i < result1.eulerCircle.size() - 1) std::cout << " -> ";
        }
        std::cout << std::endl;
    }
    std::cout << "Message: " << result1.message << std::endl;
    
    // Test 2: Graph without Euler circuit
    std::cout << "\n--- Test 2: Graph without Euler circuit ---" << std::endl;
    Graph testGraph2(3, false);
    testGraph2.addEdge(0, 1);
    testGraph2.addEdge(1, 2);
    
    GraphRequest request2(2, testGraph2);
    EulerServer::EulerResult result2 = server.processRequest(request2);
    
    std::cout << "Client: " << result2.clientId << std::endl;
    std::cout << "Has Euler Circle: " << (result2.hasEulerCircle ? "Yes" : "No") << std::endl;
    std::cout << "Message: " << result2.message << std::endl;
    
    // Test 3: Random graph
    std::cout << "\n--- Test 3: Random graph ---" << std::endl;
    auto randomGraph = GraphGenerator::generateRandomGraph(5, 7, 123);
    GraphRequest request3(3, *randomGraph);
    EulerServer::EulerResult result3 = server.processRequest(request3);
    
    std::cout << "Random graph:" << std::endl;
    randomGraph->printGraph();
    std::cout << "Client: " << result3.clientId << std::endl;
    std::cout << "Has Euler Circle: " << (result3.hasEulerCircle ? "Yes" : "No") << std::endl;
    std::cout << "Message: " << result3.message << std::endl;
    
    server.stop();
}

int main() {
    testEulerServer();
    return 0;
}