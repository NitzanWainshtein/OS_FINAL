#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H

#include "Graph.h"
#include <memory>

class GraphGenerator {
public:
    // Generate random graph with specified parameters
    static std::unique_ptr<Graph> generateRandomGraph(int vertices, int edges, 
                                                     unsigned int seed, bool directed = false);
    
    // Generate specific test graphs
    static std::unique_ptr<Graph> generateCompleteGraph(int vertices, bool directed = false);
    static std::unique_ptr<Graph> generateCycleGraph(int vertices);
    static std::unique_ptr<Graph> generatePathGraph(int vertices);
    static std::unique_ptr<Graph> generateStarGraph(int vertices);
    
    // Validation methods
    static bool isValidGraphParams(int vertices, int edges, bool directed);
    static int getMaxEdges(int vertices, bool directed);
    
private:
    static void validateParams(int vertices, int edges, bool directed);
};

#endif