#ifndef MAX_CLIQUE_ALGORITHM_H
#define MAX_CLIQUE_ALGORITHM_H

#include "AlgorithmStrategy.h"

class MaxCliqueAlgorithm : public AlgorithmStrategy {
public:
    std::string execute(const Graph& graph) override;
    std::string getAlgorithmName() const override { return "Max Clique"; }

private:
    void findMaxClique(const Graph& graph, std::vector<int>& current, 
                      std::vector<int>& candidates, std::vector<int>& maxClique, int start);
    bool areAllConnected(const Graph& graph, const std::vector<int>& vertices);
};

#endif