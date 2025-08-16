#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H

#include "Graph.h"
#include <memory>

class GraphGenerator {
public:
    static std::unique_ptr<Graph> generateRandomGraph(int vertices, int edges, 
                                                     unsigned int seed, bool directed = false);
};

#endif