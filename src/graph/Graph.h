#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <iostream>

class Graph {
private:
    int vertices;
    std::vector<std::list<int>> adjList;
    bool directed;
    
public:
    Graph(int v, bool isDirected = false);
    void addEdge(int u, int v);
    void removeEdge(int u, int v);
    std::vector<int> getNeighbors(int vertex) const;
    int getVertexCount() const { return vertices; }
    bool isDirected() const { return directed; }
    void printGraph() const;
    
    // Euler circuit methods
    bool hasEulerCircuit() const;
    std::vector<int> findEulerCircuit();
    
    // ✅ MOVED TO PUBLIC - now accessible from main.cpp
    bool isConnected() const;
    
private:
    int getDegree(int vertex) const;
    void dfsUtil(int v, std::vector<bool>& visited) const;
    void hierholzerAlgorithm(std::vector<int>& circuit);
};

#endif