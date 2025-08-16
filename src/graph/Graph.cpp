#include "Graph.h"
#include <algorithm>
#include <stack>

Graph::Graph(int v, bool isDirected) : vertices(v), directed(isDirected) {
    adjList.resize(v);
}

void Graph::addEdge(int u, int v) {
    if (u >= 0 && u < vertices && v >= 0 && v < vertices) {
        adjList[u].push_back(v);
        if (!directed) {
            adjList[v].push_back(u);
        }
    }
}

void Graph::removeEdge(int u, int v) {
    if (u >= 0 && u < vertices && v >= 0 && v < vertices) {
        adjList[u].remove(v);
        if (!directed) {
            adjList[v].remove(u);
        }
    }
}

std::vector<int> Graph::getNeighbors(int vertex) const {
    if (vertex >= 0 && vertex < vertices) {
        return std::vector<int>(adjList[vertex].begin(), adjList[vertex].end());
    }
    return std::vector<int>();
}

void Graph::printGraph() const {
    for (int i = 0; i < vertices; ++i) {
        std::cout << "Vertex " << i << ": ";
        for (int neighbor : adjList[i]) {
            std::cout << neighbor << " ";
        }
        std::cout << std::endl;
    }
}

int Graph::getDegree(int vertex) const {
    if (vertex >= 0 && vertex < vertices) {
        return adjList[vertex].size();
    }
    return 0;
}

bool Graph::isConnected() const {
    // Find a vertex with non-zero degree
    int start = -1;
    for (int i = 0; i < vertices; ++i) {
        if (getDegree(i) > 0) {
            start = i;
            break;
        }
    }
    
    if (start == -1) return true; // No edges
    
    std::vector<bool> visited(vertices, false);
    dfsUtil(start, visited);
    
    // Check if all vertices with edges are visited
    for (int i = 0; i < vertices; ++i) {
        if (getDegree(i) > 0 && !visited[i]) {
            return false;
        }
    }
    return true;
}

void Graph::dfsUtil(int v, std::vector<bool>& visited) const {
    visited[v] = true;
    for (int neighbor : adjList[v]) {
        if (!visited[neighbor]) {
            dfsUtil(neighbor, visited);
        }
    }
}

bool Graph::hasEulerCircuit() const {
    if (!isConnected()) {
        return false;
    }
    
    // For undirected graph: all vertices must have even degree
    if (!directed) {
        for (int i = 0; i < vertices; ++i) {
            if (getDegree(i) % 2 != 0) {
                return false;
            }
        }
        return true;
    }
    
    // For directed graph: in-degree must equal out-degree
    std::vector<int> inDegree(vertices, 0);
    for (int i = 0; i < vertices; ++i) {
        for (int neighbor : adjList[i]) {
            inDegree[neighbor]++;
        }
    }
    
    for (int i = 0; i < vertices; ++i) {
        if (inDegree[i] != getDegree(i)) {
            return false;
        }
    }
    return true;
}

std::vector<int> Graph::findEulerCircuit() {
    std::vector<int> circuit;
    
    if (!hasEulerCircuit()) {
        return circuit; // Empty if none exists
    }
    
    // Create working copy for modification
    Graph tempGraph = *this;
    tempGraph.hierholzerAlgorithm(circuit);
    
    return circuit;
}

void Graph::hierholzerAlgorithm(std::vector<int>& circuit) {
    // Find starting vertex with non-zero degree
    int start = 0;
    for (int i = 0; i < vertices; ++i) {
        if (getDegree(i) > 0) {
            start = i;
            break;
        }
    }
    
    std::stack<int> stack;
    stack.push(start);
    
    while (!stack.empty()) {
        int v = stack.top();
        
        if (!adjList[v].empty()) {
            int u = adjList[v].front();
            removeEdge(v, u);
            stack.push(u);
        } else {
            circuit.push_back(v);
            stack.pop();
        }
    }
    
    std::reverse(circuit.begin(), circuit.end());
}