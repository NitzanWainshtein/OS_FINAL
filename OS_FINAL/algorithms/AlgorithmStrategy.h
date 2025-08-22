#ifndef ALGORITHM_STRATEGY_H
#define ALGORITHM_STRATEGY_H

#include "../common/Graph.h"
#include <string>
#include <chrono>

enum class AlgorithmType {
    EULER_CIRCUIT,
    MST_WEIGHT,
    MAX_CLIQUE,
    HAMILTON_CIRCUIT,
    SCC
};

struct AlgorithmResult {
    bool success;
    std::string result;
    std::string error;
    std::string executionTime;
    AlgorithmType type;
    
    AlgorithmResult(AlgorithmType t) : success(false), type(t) {}
};

// Strategy Pattern - base interface
class AlgorithmStrategy {
public:
    virtual ~AlgorithmStrategy() = default;
    virtual AlgorithmResult execute(const Graph& graph) = 0;
    virtual std::string getAlgorithmName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual bool supportsDirected() const = 0;
    virtual bool supportsUndirected() const = 0;
    
protected:
    AlgorithmResult createResult(AlgorithmType type, const std::string& result);
    AlgorithmResult createError(AlgorithmType type, const std::string& error);
    void validateGraph(const Graph& graph, bool needsDirected, bool needsUndirected) const;
};

#endif