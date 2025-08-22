#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <iostream>
#include <stdexcept>

class Graph {
private:
    int vertices;
    std::vector<std::list<int>> adjList;
    bool directed;
    
public:
    // Constructor
    Graph(int v, bool isDirected = false);
    
    // Copy constructor and assignment operator
    Graph(const Graph& other);
    Graph& operator=(const Graph& other);
    
    // Basic operations
    void addEdge(int u, int v);
    void removeEdge(int u, int v);
    std::vector<int> getNeighbors(int vertex) const;
    
    // Getters
    int getVertexCount() const { return vertices; }
    bool isDirected() const { return directed; }
    
    // Display
    void printGraph() const;
    
    // Validation
    bool isValidVertex(int vertex) const;
    bool hasEdge(int u, int v) const;
    int getDegree(int vertex) const;
    
    // Graph properties
    bool isEmpty() const;
    int getEdgeCount() const;
    bool isConnected() const;
    
    // Euler circuit methods (for requirements 2-3)
    bool hasEulerCircuit() const;
    std::vector<int> findEulerCircuit();
    
    // Utility methods
    void clear();
    std::vector<std::pair<int, int>> getAllEdges() const;
    
private:
    void dfsUtil(int v, std::vector<bool>& visited) const;
    void hierholzerAlgorithm(std::vector<int>& circuit);
};

#endif