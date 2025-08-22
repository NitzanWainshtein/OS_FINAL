#include "HamiltonAlgorithm.h"
#include <algorithm>
#include <sstream>
#include <chrono>

AlgorithmResult HamiltonAlgorithm::execute(const Graph& graph) {
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        int n = graph.getVertexCount();
        
        if (n > 12) {
            return createError(AlgorithmType::HAMILTON_CIRCUIT, 
                "Graph too large for exact algorithm (max 12 vertices)");
        }
        
        if (n < 3) {
            return createResult(AlgorithmType::HAMILTON_CIRCUIT, 
                "Hamilton circuit requires at least 3 vertices");
        }
        
        std::vector<int> path(n + 1, -1);
        std::vector<bool> visited(n, false);
        
        path[0] = 0;
        visited[0] = true;
        
        if (hamiltonUtil(graph, path, visited, 1)) {
            path[n] = path[0]; // Complete the circuit
            
            std::ostringstream ss;
            ss << "Hamilton circuit: ";
            for (int i = 0; i <= n; ++i) {
                ss << path[i];
                if (i < n) ss << "→";
            }
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
            AlgorithmResult result = createResult(AlgorithmType::HAMILTON_CIRCUIT, ss.str());
            result.executionTime = std::to_string(duration.count()) + "μs";
            return result;
        } else {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
            AlgorithmResult result = createResult(AlgorithmType::HAMILTON_CIRCUIT, 
                "No Hamilton circuit exists");
            result.executionTime = std::to_string(duration.count()) + "μs";
            return result;
        }
        
    } catch (const std::exception& e) {
        return createError(AlgorithmType::HAMILTON_CIRCUIT, e.what());
    }
}

bool HamiltonAlgorithm::hamiltonUtil(const Graph& graph, std::vector<int>& path,
                                    std::vector<bool>& visited, int pos) {
    int n = graph.getVertexCount();
    
    if (pos == n) {
        // Check if there's an edge from last vertex back to first
        auto lastVertexNeighbors = graph.getNeighbors(path[pos - 1]);
        return std::find(lastVertexNeighbors.begin(), lastVertexNeighbors.end(), path[0]) 
               != lastVertexNeighbors.end();
    }
    
    for (int v = 1; v < n; ++v) {
        if (isSafe(graph, v, path, pos) && !visited[v]) {
            path[pos] = v;
            visited[v] = true;
            
            if (hamiltonUtil(graph, path, visited, pos + 1)) {
                return true;
            }
            
            path[pos] = -1;
            visited[v] = false;
        }
    }
    
    return false;
}

bool HamiltonAlgorithm::isSafe(const Graph& graph, int v, 
                              const std::vector<int>& path, int pos) {
    auto neighbors = graph.getNeighbors(path[pos - 1]);
    return std::find(neighbors.begin(), neighbors.end(), v) != neighbors.end();
}