#ifndef MST_ALGORITHM_H
#define MST_ALGORITHM_H

#include "AlgorithmStrategy.h"

class MSTAlgorithm : public AlgorithmStrategy {
public:
    AlgorithmResult execute(const Graph& graph) override;
    std::string getAlgorithmName() const override { return "MST Weight"; }
    std::string getDescription() const override { 
        return "Finds Minimum Spanning Tree weight using Kruskal's algorithm"; 
    }
    bool supportsDirected() const override { return false; }
    bool supportsUndirected() const override { return true; }

private:
    struct Edge {
        int u, v, weight;
        Edge(int u, int v, int w) : u(u), v(v), weight(w) {}
        bool operator<(const Edge& other) const { return weight < other.weight; }
    };
    
    int find(std::vector<int>& parent, int i);
    void unite(std::vector<int>& parent, std::vector<int>& rank, int x, int y);
};

#endif