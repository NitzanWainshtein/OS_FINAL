#ifndef MST_ALGORITHM_H
#define MST_ALGORITHM_H

#include "AlgorithmStrategy.h"

class MSTAlgorithm : public AlgorithmStrategy {
public:
    std::string execute(const Graph& graph) override;
    std::string getAlgorithmName() const override { return "MST Weight"; }

private:
    struct Edge {
        int src, dest, weight;
        Edge(int s, int d, int w) : src(s), dest(d), weight(w) {}
        bool operator<(const Edge& other) const { return weight < other.weight; }
    };
    
    int find(std::vector<int>& parent, int i);
    void unite(std::vector<int>& parent, std::vector<int>& rank, int x, int y);
};

#endif