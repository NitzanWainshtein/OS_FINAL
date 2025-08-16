#ifndef SCC_ALGORITHM_H
#define SCC_ALGORITHM_H

#include "AlgorithmStrategy.h"
#include <stack>

class SCCAlgorithm : public AlgorithmStrategy {
public:
    std::string execute(const Graph& graph) override;
    std::string getAlgorithmName() const override { return "Strongly Connected Components"; }

private:
    void dfs1(const Graph& graph, int v, std::vector<bool>& visited, std::stack<int>& finishOrder);
    void dfs2(const Graph& transposeGraph, int v, std::vector<bool>& visited, std::vector<int>& component);
    Graph transposeGraph(const Graph& graph);
};

#endif