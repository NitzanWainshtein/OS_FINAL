#include "GraphGenerator.h"
#include <random>
#include <algorithm>
#include <stdexcept>
#include <iostream>

std::unique_ptr<Graph> GraphGenerator::generateRandomGraph(int vertices, int edges, 
                                                          unsigned int seed, bool directed) {
    validateParams(vertices, edges, directed);
    
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, vertices - 1);
    
    auto graph = std::make_unique<Graph>(vertices, directed);
    
    if (edges == 0) {
        return graph; // Return empty graph
    }
    
    int edgesAdded = 0;
    int maxAttempts = edges * 20; // Prevent infinite loops
    int attempts = 0;
    
    std::cout << "Generating random graph: " << vertices << " vertices, " 
              << edges << " edges, seed=" << seed << std::endl;
    
    while (edgesAdded < edges && attempts < maxAttempts) {
        int u = dis(gen);
        int v = dis(gen);
        
        if (u != v && !graph->hasEdge(u, v)) {
            try {
                graph->addEdge(u, v);
                edgesAdded++;
                
                if (edgesAdded % 10 == 0 || edgesAdded == edges) {
                    std::cout << "Progress: " << edgesAdded << "/" << edges << " edges added" << std::endl;
                }
            } catch (const std::exception& e) {
                // Edge already exists or invalid, continue
            }
        }
        attempts++;
    }
    
    if (edgesAdded < edges) {
        std::cout << "Warning: Could only add " << edgesAdded << "/" << edges 
                  << " edges (graph may be getting saturated)" << std::endl;
    }
    
    return graph;
}

std::unique_ptr<Graph> GraphGenerator::generateCompleteGraph(int vertices, bool directed) {
    if (vertices <= 0 || vertices > 1000) {
        throw std::invalid_argument("Vertices must be between 1-1000 for complete graph");
    }
    
    auto graph = std::make_unique<Graph>(vertices, directed);
    
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            if (i != j) {
                graph->addEdge(i, j);
            }
        }
    }
    
    std::cout << "Generated complete K" << vertices << " graph (" 
              << (directed ? "directed" : "undirected") << ")" << std::endl;
    
    return graph;
}

std::unique_ptr<Graph> GraphGenerator::generateCycleGraph(int vertices) {
    if (vertices < 3) {
        throw std::invalid_argument("Cycle graph needs at least 3 vertices");
    }
    
    auto graph = std::make_unique<Graph>(vertices, false);
    
    for (int i = 0; i < vertices; ++i) {
        graph->addEdge(i, (i + 1) % vertices);
    }
    
    std::cout << "Generated cycle graph C" << vertices << std::endl;
    return graph;
}

std::unique_ptr<Graph> GraphGenerator::generatePathGraph(int vertices) {
    if (vertices < 2) {
        throw std::invalid_argument("Path graph needs at least 2 vertices");
    }
    
    auto graph = std::make_unique<Graph>(vertices, false);
    
    for (int i = 0; i < vertices - 1; ++i) {
        graph->addEdge(i, i + 1);
    }
    
    std::cout << "Generated path graph P" << vertices << std::endl;
    return graph;
}

std::unique_ptr<Graph> GraphGenerator::generateStarGraph(int vertices) {
    if (vertices < 2) {
        throw std::invalid_argument("Star graph needs at least 2 vertices");
    }
    
    auto graph = std::make_unique<Graph>(vertices, false);
    
    // Connect vertex 0 to all others
    for (int i = 1; i < vertices; ++i) {
        graph->addEdge(0, i);
    }
    
    std::cout << "Generated star graph S" << vertices << std::endl;
    return graph;
}

bool GraphGenerator::isValidGraphParams(int vertices, int edges, bool directed) {
    if (vertices <= 0 || vertices > 10000) return false;
    if (edges < 0) return false;
    
    int maxEdges = getMaxEdges(vertices, directed);
    return edges <= maxEdges;
}

int GraphGenerator::getMaxEdges(int vertices, bool directed) {
    if (vertices <= 1) return 0;
    return directed ? vertices * (vertices - 1) : vertices * (vertices - 1) / 2;
}

void GraphGenerator::validateParams(int vertices, int edges, bool directed) {
    if (vertices <= 0) {
        throw std::invalid_argument("Number of vertices must be positive, got: " + std::to_string(vertices));
    }
    if (vertices > 10000) {
        throw std::invalid_argument("Number of vertices too large (max 10000), got: " + std::to_string(vertices));
    }
    if (edges < 0) {
        throw std::invalid_argument("Number of edges cannot be negative, got: " + std::to_string(edges));
    }
    
    int maxEdges = getMaxEdges(vertices, directed);
    if (edges > maxEdges) {
        throw std::invalid_argument("Too many edges for " + std::to_string(vertices) + 
                                  " vertices. Max allowed: " + std::to_string(maxEdges) + 
                                  ", requested: " + std::to_string(edges));
    }
}