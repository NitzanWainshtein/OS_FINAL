#include "AlgorithmsServer.h"
#include "../graph/GraphGenerator.h"
#include <iostream>

void testAlgorithmsServer() {
    std::cout << "=== Testing Algorithms Server (Requirement 7) ===" << std::endl;
    
    AlgorithmsServer server(8080);
    server.start();
    
    // Create test graph - complete graph K4
    std::cout << "\n--- Creating Test Graph (Complete K4) ---" << std::endl;
    Graph testGraph(4, false);
    testGraph.addEdge(0, 1);
    testGraph.addEdge(0, 2);
    testGraph.addEdge(0, 3);
    testGraph.addEdge(1, 2);
    testGraph.addEdge(1, 3);
    testGraph.addEdge(2, 3);
    
    std::cout << "Test Graph:" << std::endl;
    testGraph.printGraph();
    
    // Test all 4 algorithms
    std::vector<AlgorithmType> algorithms = {
        AlgorithmType::EULER_CIRCUIT,
        AlgorithmType::MST_WEIGHT,
        AlgorithmType::MAX_CLIQUE,
        AlgorithmType::HAMILTON_CIRCUIT
    };
    
    int clientId = 1;
    for (auto algType : algorithms) {
        std::cout << "\n=== Testing " << AlgorithmFactory::typeToString(algType) << " ===" << std::endl;
        
        AlgorithmRequest request(clientId++, algType, testGraph);
        AlgorithmResult result = server.processRequest(request);
        
        std::cout << "Client ID: " << result.clientId << std::endl;
        std::cout << "Algorithm: " << AlgorithmFactory::typeToString(result.algorithmType) << std::endl;
        std::cout << "Success: " << (result.success ? "Yes" : "No") << std::endl;
        
        if (result.success) {
            std::cout << "Result: " << result.result << std::endl;
        } else {
            std::cout << "Error: " << result.error << std::endl;
        }
    }
    
    // Test with directed graph for SCC
    std::cout << "\n=== Testing SCC with Directed Graph ===" << std::endl;
    Graph directedGraph(4, true); // directed
    directedGraph.addEdge(0, 1);
    directedGraph.addEdge(1, 2);
    directedGraph.addEdge(2, 0);
    directedGraph.addEdge(2, 3);
    
    std::cout << "Directed Graph:" << std::endl;
    directedGraph.printGraph();
    
    AlgorithmRequest sccRequest(5, AlgorithmType::SCC, directedGraph);
    AlgorithmResult sccResult = server.processRequest(sccRequest);
    
    std::cout << "SCC Result: " << (sccResult.success ? sccResult.result : sccResult.error) << std::endl;
    
    // Test with random graph
    std::cout << "\n=== Testing with Random Graph ===" << std::endl;
    auto randomGraph = GraphGenerator::generateRandomGraph(5, 7, 456);
    std::cout << "Random Graph:" << std::endl;
    randomGraph->printGraph();
    
    AlgorithmRequest randomRequest(6, AlgorithmType::EULER_CIRCUIT, *randomGraph);
    AlgorithmResult randomResult = server.processRequest(randomRequest);
    
    std::cout << "Random Graph Euler Result: " << (randomResult.success ? randomResult.result : randomResult.error) << std::endl;
    
    server.stop();
}

int main() {
    testAlgorithmsServer();
    return 0;
}