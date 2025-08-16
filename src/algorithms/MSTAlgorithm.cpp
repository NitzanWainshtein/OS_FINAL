#include "MSTAlgorithm.h"
#include <algorithm>
#include <random>
#include <sstream>

std::string MSTAlgorithm::execute(const Graph& graph) {
    if (graph.isDirected()) {
        return "MST not applicable to directed graphs";
    }
    
    int n = graph.getVertexCount();
    std::vector<Edge> edges;
    
    // Generate edges with random weights (for demo purposes)
    std::mt19937 gen(42);
    std::uniform_int_distribution<> weightDist(1, 100);
    
    for (int i = 0; i < n; ++i) {
        auto neighbors = graph.getNeighbors(i);
        for (int neighbor : neighbors) {
            if (i < neighbor) { // Avoid duplicates in undirected graph
                edges.emplace_back(i, neighbor, weightDist(gen));
            }
        }
    }
    
    if (edges.empty()) {
        return "No edges in graph";
    }
    
    // Kruskal's algorithm
    std::sort(edges.begin(), edges.end());
    
    std::vector<int> parent(n);
    std::vector<int> rank(n, 0);
    for (int i = 0; i < n; ++i) parent[i] = i;
    
    int mstWeight = 0;
    int edgesUsed = 0;
    
    for (const auto& edge : edges) {
        int rootSrc = find(parent, edge.src);
        int rootDest = find(parent, edge.dest);
        
        if (rootSrc != rootDest) {
            mstWeight += edge.weight;
            unite(parent, rank, rootSrc, rootDest);
            edgesUsed++;
            
            if (edgesUsed == n - 1) break;
        }
    }
    
    std::stringstream ss;
    ss << "MST weight: " << mstWeight;
    return ss.str();
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