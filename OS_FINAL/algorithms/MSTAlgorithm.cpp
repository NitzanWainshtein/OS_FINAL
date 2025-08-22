#include "MSTAlgorithm.h"
#include <algorithm>
#include <random>
#include <sstream>
#include <chrono>

AlgorithmResult MSTAlgorithm::execute(const Graph& graph) {
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        validateGraph(graph, false, true);
        
        if (graph.getEdgeCount() == 0) {
            return createResult(AlgorithmType::MST_WEIGHT, "MST weight: 0 (no edges)");
        }
        
        if (!graph.isConnected()) {
            return createResult(AlgorithmType::MST_WEIGHT, "No MST (graph not connected)");
        }
        
        // Generate random weights for edges
        std::vector<Edge> edges;
        std::mt19937 gen(42); // Fixed seed for reproducibility
        std::uniform_int_distribution<> weightDist(1, 10);
        
        auto allEdges = graph.getAllEdges();
        for (const auto& edge : allEdges) {
            edges.emplace_back(edge.first, edge.second, weightDist(gen));
        }
        
        // Kruskal's algorithm
        std::sort(edges.begin(), edges.end());
        
        int n = graph.getVertexCount();
        std::vector<int> parent(n);
        std::vector<int> rank(n, 0);
        for (int i = 0; i < n; ++i) parent[i] = i;
        
        int mstWeight = 0;
        int edgesUsed = 0;
        
        for (const auto& edge : edges) {
            int rootU = find(parent, edge.u);
            int rootV = find(parent, edge.v);
            
            if (rootU != rootV) {
                mstWeight += edge.weight;
                unite(parent, rank, rootU, rootV);
                edgesUsed++;
                
                if (edgesUsed == n - 1) break;
            }
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::ostringstream ss;
        ss << "MST weight: " << mstWeight << " (edges: " << edgesUsed << ")";
        
        AlgorithmResult result = createResult(AlgorithmType::MST_WEIGHT, ss.str());
        result.executionTime = std::to_string(duration.count()) + "μs";
        return result;
        
    } catch (const std::exception& e) {
        return createError(AlgorithmType::MST_WEIGHT, e.what());
    }
}

int MSTAlgorithm::find(std::vector<int>& parent, int i) {
    if (parent[i] != i) {
        parent[i] = find(parent, parent[i]);
    }
    return parent[i];
}

void MSTAlgorithm::unite(std::vector<int>& parent, std::vector<int>& rank, int x, int y) {
    if (rank[x] < rank[y]) {
        parent[x] = y;
    } else if (rank[x] > rank[y]) {
        parent[y] = x;
    } else {
        parent[y] = x;
        rank[x]++;
    }
}