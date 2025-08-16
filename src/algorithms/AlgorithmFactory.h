#ifndef ALGORITHM_FACTORY_H
#define ALGORITHM_FACTORY_H

#include "AlgorithmStrategy.h"
#include <memory>

// Factory Pattern
class AlgorithmFactory {
public:
    static std::unique_ptr<AlgorithmStrategy> createAlgorithm(AlgorithmType type);
    static AlgorithmType stringToType(const std::string& str);
    static std::string typeToString(AlgorithmType type);
};

#endif