#include "SCCAlgorithm.h"
#include <algorithm>
#include <sstream>

std::string SCCAlgorithm::execute(const Graph& graph) {
    if (!graph.isDirected()) {
        return "SCC only applicable to directed graphs";
    }
    
    int n = graph.getVertexCount();
    std::vector<bool> visited(n, false);
    std::stack<int> finishOrder;
    
    // Step 1: DFS on original graph to get finish order
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            dfs1(graph, i, visited, finishOrder);
        }
    }
    
    // Step 2: Create transpose graph
    Graph transposed = transposeGraph(graph);
    
    // Step 3: DFS on transpose graph in reverse finish order
    std::fill(visited.begin(), visited.end(), false);
    std::vector<std::vector<int>> sccs;
    
    while (!finishOrder.empty()) {
        int v = finishOrder.top();
        finishOrder.pop();
        
        if (!visited[v]) {
            std::vector<int> component;
            dfs2(transposed, v, visited, component);
            sccs.push_back(component);
        }
    }
    
    // Format result
    std::stringstream ss;
    ss << "Number of SCCs: " << sccs.size() << "\nSCCs: ";
    for (const auto& scc : sccs) {
        ss << "[";
        for (size_t i = 0; i < scc.size(); ++i) {
            ss << scc[i];
            if (i < scc.size() - 1) ss << " ";
        }
        ss << "] ";
    }
    return ss.str();
}

void SCCAlgorithm::dfs1(const Graph& graph, int v, std::vector<bool>& visited, std::stack<int>& finishOrder) {
    visited[v] = true;
    
    auto neighbors = graph.getNeighbors(v);
    for (int neighbor : neighbors) {
        if (!visited[neighbor]) {
            dfs1(graph, neighbor, visited, finishOrder);
        }
    }
    
    // Add to stack when finishing (post-order)
    finishOrder.push(v);
}

void SCCAlgorithm::dfs2(const Graph& transposeGraph, int v, std::vector<bool>& visited, std::vector<int>& component) {
    visited[v] = true;
    component.push_back(v);
    
    auto neighbors = transposeGraph.getNeighbors(v);
    for (int neighbor : neighbors) {
        if (!visited[neighbor]) {
            dfs2(transposeGraph, neighbor, visited, component);
        }
    }
}

Graph SCCAlgorithm::transposeGraph(const Graph& graph) {
    int n = graph.getVertexCount();
    Graph transposed(n, true); // Create directed graph
    
    // Reverse all edges
    for (int i = 0; i < n; ++i) {
        auto neighbors = graph.getNeighbors(i);
        for (int neighbor : neighbors) {
            transposed.addEdge(neighbor, i); // Reverse the edge
        }
    }
    
    return transposed;
}
