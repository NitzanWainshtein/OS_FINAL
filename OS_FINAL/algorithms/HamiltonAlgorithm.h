#ifndef HAMILTON_ALGORITHM_H
#define HAMILTON_ALGORITHM_H

#include "AlgorithmStrategy.h"

class HamiltonAlgorithm : public AlgorithmStrategy {
public:
    AlgorithmResult execute(const Graph& graph) override;
    std::string getAlgorithmName() const override { return "Hamilton Circuit"; }
    std::string getDescription() const override { 
        return "Finds Hamilton circuit (visits every vertex exactly once)"; 
    }
    bool supportsDirected() const override { return true; }
    bool supportsUndirected() const override { return true; }

private:
    bool hamiltonUtil(const Graph& graph, std::vector<int>& path, 
                     std::vector<bool>& visited, int pos);
    bool isSafe(const Graph& graph, int v, const std::vector<int>& path, int pos);
};

#endif
