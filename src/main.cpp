#include "graph/Graph.h"
#include "graph/GraphGenerator.h"
#include <iostream>
#include <getopt.h>

void printUsage(const char* progName) {
    std::cout << "Usage: " << progName << " -v <vertices> -e <edges> -s <seed>" << std::endl;
    std::cout << "  -v, --vertices  Number of vertices" << std::endl;
    std::cout << "  -e, --edges     Number of edges" << std::endl;
    std::cout << "  -s, --seed      Random seed" << std::endl;
    std::cout << "  -h, --help      Show this help message" << std::endl;
}

int main(int argc, char* argv[]) {
    int vertices = 5;
    int edges = 6;
    unsigned int seed = 42;
    
    static struct option long_options[] = {
        {"vertices", required_argument, 0, 'v'},
        {"edges", required_argument, 0, 'e'},
        {"seed", required_argument, 0, 's'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    int opt;
    while ((opt = getopt_long(argc, argv, "v:e:s:h", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'v': vertices = std::atoi(optarg); break;
            case 'e': edges = std::atoi(optarg); break;
            case 's': seed = std::atoi(optarg); break;
            case 'h': printUsage(argv[0]); return 0;
            default: printUsage(argv[0]); return 1;
        }
    }
    
    std::cout << "=== Graph Algorithms Project - Requirements 1-4 ===" << std::endl;
    std::cout << "Generating random graph with " << vertices << " vertices, " 
              << edges << " edges, seed: " << seed << std::endl;
    
    auto graph = GraphGenerator::generateRandomGraph(vertices, edges, seed);
    
    std::cout << "\nGenerated Graph:" << std::endl;
    graph->printGraph();
    
    std::cout << "\n=== Euler Circuit Analysis ===" << std::endl;
    if (graph->hasEulerCircuit()) {
        std::cout << "✓ Euler circuit exists!" << std::endl;
        auto circuit = graph->findEulerCircuit();
        
        std::cout << "Euler circuit: ";
        for (size_t i = 0; i < circuit.size(); ++i) {
            std::cout << circuit[i];
            if (i < circuit.size() - 1) std::cout << " -> ";
        }
        std::cout << std::endl;
        std::cout << "Circuit length: " << circuit.size() << " vertices" << std::endl;
    } else {
        std::cout << "✗ No Euler circuit exists." << std::endl;
        std::cout << "Reason: ";
        
        // Detailed explanation
        bool connected = graph->isConnected();
        if (!connected) {
            std::cout << "Graph is not connected." << std::endl;
        } else {
            std::cout << "Not all vertices have even degree." << std::endl;
            for (int i = 0; i < graph->getVertexCount(); ++i) {
                auto neighbors = graph->getNeighbors(i);
                int degree = neighbors.size();
                if (degree % 2 != 0) {
                    std::cout << "  Vertex " << i << " has odd degree: " << degree << std::endl;
                }
            }
        }
    }
    
    return 0;
}