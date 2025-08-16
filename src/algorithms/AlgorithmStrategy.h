#ifndef ALGORITHM_STRATEGY_H
#define ALGORITHM_STRATEGY_H
#include "../graph/Graph.h"
#include <string>

enum class AlgorithmType {
    EULER_CIRCUIT,
    MST_WEIGHT,
    MAX_CLIQUE,
    HAMILTON_CIRCUIT,
    SCC
};

// Strategy Pattern - base interface
class AlgorithmStrategy {
public:
    virtual ~AlgorithmStrategy() = default;
    virtual std::string execute(const Graph& graph) = 0;
    virtual std::string getAlgorithmName() const = 0;
};

#endif