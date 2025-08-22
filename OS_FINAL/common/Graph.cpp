#include "Graph.h"
#include <algorithm>
#include <stack>
#include <iomanip>

Graph::Graph(int v, bool isDirected) : vertices(v), directed(isDirected) {
    if (v <= 0) {
        throw std::invalid_argument("Number of vertices must be positive, got: " + std::to_string(v));
    }
    if (v > 10000) {
        throw std::invalid_argument("Number of vertices too large (max 10000), got: " + std::to_string(v));
    }
    adjList.resize(v);
}

Graph::Graph(const Graph& other) 
    : vertices(other.vertices), directed(other.directed), adjList(other.adjList) {
}

Graph& Graph::operator=(const Graph& other) {
    if (this != &other) {
        vertices = other.vertices;
        directed = other.directed;
        adjList = other.adjList;
    }
    return *this;
}

void Graph::addEdge(int u, int v) {
    if (!isValidVertex(u)) {
        throw std::out_of_range("Vertex u=" + std::to_string(u) + " is out of range [0," + std::to_string(vertices-1) + "]");
    }
    if (!isValidVertex(v)) {
        throw std::out_of_range("Vertex v=" + std::to_string(v) + " is out of range [0," + std::to_string(vertices-1) + "]");
    }
    if (u == v) {
        throw std::invalid_argument("Self-loops not allowed (u=" + std::to_string(u) + ", v=" + std::to_string(v) + ")");
    }
    
    // Check if edge already exists
    if (hasEdge(u, v)) {
        return; // Silently ignore duplicate edges
    }
    
    adjList[u].push_back(v);
    if (!directed) {
        adjList[v].push_back(u);
    }
}

void Graph::removeEdge(int u, int v) {
    if (!isValidVertex(u) || !isValidVertex(v)) {
        return; // Silently ignore invalid vertices
    }
    
    adjList[u].remove(v);
    if (!directed) {
        adjList[v].remove(u);
    }
}

std::vector<int> Graph::getNeighbors(int vertex) const {
    if (!isValidVertex(vertex)) {
        return std::vector<int>();
    }
    return std::vector<int>(adjList[vertex].begin(), adjList[vertex].end());
}

bool Graph::isValidVertex(int vertex) const {
    return vertex >= 0 && vertex < vertices;
}

bool Graph::hasEdge(int u, int v) const {
    if (!isValidVertex(u) || !isValidVertex(v)) {
        return false;
    }
    
    const auto& neighbors = adjList[u];
    return std::find(neighbors.begin(), neighbors.end(), v) != neighbors.end();
}

int Graph::getDegree(int vertex) const {
    if (!isValidVertex(vertex)) {
        return 0;
    }
    return adjList[vertex].size();
}

bool Graph::isEmpty() const {
    for (int i = 0; i < vertices; ++i) {
        if (!adjList[i].empty()) {
            return false;
        }
    }
    return true;
}

int Graph::getEdgeCount() const {
    int count = 0;
    for (int i = 0; i < vertices; ++i) {
        count += adjList[i].size();
    }
    return directed ? count : count / 2;
}

void Graph::clear() {
    for (auto& list : adjList) {
        list.clear();
    }
}

std::vector<std::pair<int, int>> Graph::getAllEdges() const {
    std::vector<std::pair<int, int>> edges;
    
    for (int u = 0; u < vertices; ++u) {
        for (int v : adjList[u]) {
            if (directed || u < v) { // Avoid duplicates in undirected graphs
                edges.emplace_back(u, v);
            }
        }
    }
    
    return edges;
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
    
    if (start == -1) return true; // No edges - considered connected
    
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
    
    // For directed graph: in-degree must equal out-degree for all vertices
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

void Graph::printGraph() const {
    std::cout << "\n=== Graph Information ===" << std::endl;
    std::cout << "Type: " << (directed ? "Directed" : "Undirected") << std::endl;
    std::cout << "Vertices: " << vertices << std::endl;
    std::cout << "Edges: " << getEdgeCount() << std::endl;
    
    if (isEmpty()) {
        std::cout << "Graph is empty (no edges)" << std::endl;
        std::cout << "=========================" << std::endl;
        return;
    }
    
    std::cout << "\nAdjacency List:" << std::endl;
    for (int i = 0; i < vertices; ++i) {
        std::cout << "Vertex " << std::setw(2) << i << ": ";
        if (adjList[i].empty()) {
            std::cout << "(isolated)";
        } else {
            bool first = true;
            for (int neighbor : adjList[i]) {
                if (!first) std::cout << ", ";
                std::cout << neighbor;
                first = false;
            }
        }
        std::cout << " (degree: " << getDegree(i) << ")" << std::endl;
    }
    std::cout << "=========================" << std::endl;
}