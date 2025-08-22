#ifndef EULER_ALGORITHM_H
#define EULER_ALGORITHM_H

#include "AlgorithmStrategy.h"

class EulerAlgorithm : public AlgorithmStrategy {
public:
    AlgorithmResult execute(const Graph& graph) override;
    std::string getAlgorithmName() const override { return "Euler Circuit"; }
    std::string getDescription() const override { 
        return "Finds Euler circuit (visits every edge exactly once)"; 
    }
    bool supportsDirected() const override { return true; }
    bool supportsUndirected() const override { return true; }
};

#endif