#include "SCCAlgorithm.h"
#include <algorithm>
#include <sstream>
#include <chrono>

AlgorithmResult SCCAlgorithm::execute(const Graph& graph) {
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        validateGraph(graph, true, false);
        
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
        std::ostringstream ss;
        ss << "SCCs: " << sccs.size() << " components: ";
        for (size_t i = 0; i < sccs.size(); ++i) {
            ss << "{";
            for (size_t j = 0; j < sccs[i].size(); ++j) {
                ss << sccs[i][j];
                if (j < sccs[i].size() - 1) ss << ",";
            }
            ss << "}";
            if (i < sccs.size() - 1) ss << " ";
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        AlgorithmResult result = createResult(AlgorithmType::SCC, ss.str());
        result.executionTime = std::to_string(duration.count()) + "μs";
        return result;
        
    } catch (const std::exception& e) {
        return createError(AlgorithmType::SCC, e.what());
    }
}

void SCCAlgorithm::dfs1(const Graph& graph, int v, std::vector<bool>& visited, 
                       std::stack<int>& finishOrder) {
    visited[v] = true;
    
    auto neighbors = graph.getNeighbors(v);
    for (int neighbor : neighbors) {
        if (!visited[neighbor]) {
            dfs1(graph, neighbor, visited, finishOrder);
        }
    }
    
    finishOrder.push(v);
}

void SCCAlgorithm::dfs2(const Graph& transposeGraph, int v, std::vector<bool>& visited, 
                       std::vector<int>& component) {
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
    Graph transposed(n, true);
    
    for (int i = 0; i < n; ++i) {
        auto neighbors = graph.getNeighbors(i);
        for (int neighbor : neighbors) {
            transposed.addEdge(neighbor, i);
        }
    }
    
    return transposed;
}

// Implement base class methods
AlgorithmResult AlgorithmStrategy::createResult(AlgorithmType type, const std::string& result) {
    AlgorithmResult res(type);
    res.success = true;
    res.result = result;
    return res;
}

AlgorithmResult AlgorithmStrategy::createError(AlgorithmType type, const std::string& error) {
    AlgorithmResult res(type);
    res.success = false;
    res.error = error;
    return res;
}

void AlgorithmStrategy::validateGraph(const Graph& graph, bool needsDirected, bool needsUndirected) const {
    if (graph.getVertexCount() <= 0) {
        throw std::invalid_argument("Graph has no vertices");
    }
    
    if (needsDirected && !graph.isDirected()) {
        throw std::invalid_argument("Algorithm requires directed graph");
    }
    
    if (needsUndirected && graph.isDirected()) {
        throw std::invalid_argument("Algorithm requires undirected graph");
    }
}