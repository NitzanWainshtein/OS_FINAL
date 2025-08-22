#include "AlgorithmFactory.h"
#include "EulerAlgorithm.h"
#include "MaxCliqueAlgorithm.h"
#include "MSTAlgorithm.h"
#include "HamiltonAlgorithm.h"
#include "SCCAlgorithm.h"
#include <stdexcept>

std::unique_ptr<AlgorithmStrategy> AlgorithmFactory::createAlgorithm(AlgorithmType type) {
    switch (type) {
        case AlgorithmType::EULER_CIRCUIT:
            return std::make_unique<EulerAlgorithm>();
        case AlgorithmType::MAX_CLIQUE:
            return std::make_unique<MaxCliqueAlgorithm>();
        case AlgorithmType::MST_WEIGHT:
            return std::make_unique<MSTAlgorithm>();
        case AlgorithmType::HAMILTON_CIRCUIT:
            return std::make_unique<HamiltonAlgorithm>();
        case AlgorithmType::SCC:
            return std::make_unique<SCCAlgorithm>();
        default:
            throw std::invalid_argument("Unknown algorithm type");
    }
}

AlgorithmType AlgorithmFactory::stringToType(const std::string& str) {
    if (str == "euler") return AlgorithmType::EULER_CIRCUIT;
    if (str == "clique") return AlgorithmType::MAX_CLIQUE;
    if (str == "mst") return AlgorithmType::MST_WEIGHT;
    if (str == "hamilton") return AlgorithmType::HAMILTON_CIRCUIT;
    if (str == "scc") return AlgorithmType::SCC;
    throw std::invalid_argument("Unknown algorithm: " + str);
}

std::string AlgorithmFactory::typeToString(AlgorithmType type) {
    switch (type) {
        case AlgorithmType::EULER_CIRCUIT: return "euler";
        case AlgorithmType::MAX_CLIQUE: return "clique";
        case AlgorithmType::MST_WEIGHT: return "mst";
        case AlgorithmType::HAMILTON_CIRCUIT: return "hamilton";
        case AlgorithmType::SCC: return "scc";
        default: return "unknown";
    }
}

std::vector<std::string> AlgorithmFactory::getAllAlgorithmNames() {
    return {"euler", "clique", "mst", "hamilton", "scc"};
}

std::vector<AlgorithmType> AlgorithmFactory::getAllAlgorithmTypes() {
    return {AlgorithmType::EULER_CIRCUIT, AlgorithmType::MAX_CLIQUE, 
            AlgorithmType::MST_WEIGHT, AlgorithmType::HAMILTON_CIRCUIT, AlgorithmType::SCC};
}

bool AlgorithmFactory::isValidAlgorithm(const std::string& name) {
    try {
        stringToType(name);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}