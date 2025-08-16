#ifndef EULER_ALGORITHM_H
#define EULER_ALGORITHM_H

#include "AlgorithmStrategy.h"

class EulerAlgorithm : public AlgorithmStrategy {
public:
    std::string execute(const Graph& graph) override;
    std::string getAlgorithmName() const override { return "Euler Circuit"; }
};

#endif