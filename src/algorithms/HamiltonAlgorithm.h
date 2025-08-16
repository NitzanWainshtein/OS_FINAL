#ifndef HAMILTON_ALGORITHM_H
#define HAMILTON_ALGORITHM_H

#include "AlgorithmStrategy.h"

class HamiltonAlgorithm : public AlgorithmStrategy {
public:
    std::string execute(const Graph& graph) override;
    std::string getAlgorithmName() const override { return "Hamilton Circuit"; }

private:
    bool hamiltonUtil(const Graph& graph, std::vector<int>& path, 
                     std::vector<bool>& visited, int pos);
    bool isSafe(const Graph& graph, int v, const std::vector<int>& path, int pos);
};

#endif