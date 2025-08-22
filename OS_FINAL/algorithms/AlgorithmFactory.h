#ifndef ALGORITHM_FACTORY_H
#define ALGORITHM_FACTORY_H

#include "AlgorithmStrategy.h"
#include <memory>
#include <string>
#include <vector>

// Factory Pattern
class AlgorithmFactory {
public:
    static std::unique_ptr<AlgorithmStrategy> createAlgorithm(AlgorithmType type);
    static AlgorithmType stringToType(const std::string& str);
    static std::string typeToString(AlgorithmType type);
    static std::vector<std::string> getAllAlgorithmNames();
    static std::vector<AlgorithmType> getAllAlgorithmTypes();
    static bool isValidAlgorithm(const std::string& name);
};

#endif