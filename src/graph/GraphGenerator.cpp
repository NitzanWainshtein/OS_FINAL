#include "GraphGenerator.h"
#include <random>
#include <algorithm>

std::unique_ptr<Graph> GraphGenerator::generateRandomGraph(int vertices, int edges, 
                                                          unsigned int seed, bool directed) {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, vertices - 1);
    
    auto graph = std::make_unique<Graph>(vertices, directed);
    
    int edgesAdded = 0;
    int maxAttempts = edges * 10; // Prevent infinite loop
    int attempts = 0;
    
    while (edgesAdded < edges && attempts < maxAttempts) {
        int u = dis(gen);
        int v = dis(gen);
        
        if (u != v) { // No self-loops
            // Check if edge already exists
            auto neighbors = graph->getNeighbors(u);
            if (std::find(neighbors.begin(), neighbors.end(), v) == neighbors.end()) {
                graph->addEdge(u, v);
                edgesAdded++;
            }
        }
        attempts++;
    }
    
    return graph;
}