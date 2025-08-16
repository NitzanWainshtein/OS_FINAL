#include "AlgorithmFactory.h"
#include "EulerAlgorithm.h"
#include "MSTAlgorithm.h"
#include "MaxCliqueAlgorithm.h"
#include "HamiltonAlgorithm.h"
#include "SCCAlgorithm.h"

std::unique_ptr<AlgorithmStrategy> AlgorithmFactory::createAlgorithm(AlgorithmType type) {
    switch (type) {
        case AlgorithmType::EULER_CIRCUIT:
            return std::make_unique<EulerAlgorithm>();
        case AlgorithmType::MST_WEIGHT:
            return std::make_unique<MSTAlgorithm>();
        case AlgorithmType::MAX_CLIQUE:
            return std::make_unique<MaxCliqueAlgorithm>();
        case AlgorithmType::HAMILTON_CIRCUIT:
            return std::make_unique<HamiltonAlgorithm>();
        case AlgorithmType::SCC:
            return std::make_unique<SCCAlgorithm>();
        default:
            return nullptr;
    }
}

AlgorithmType AlgorithmFactory::stringToType(const std::string& str) {
    if (str == "euler") return AlgorithmType::EULER_CIRCUIT;
    if (str == "mst") return AlgorithmType::MST_WEIGHT;
    if (str == "clique") return AlgorithmType::MAX_CLIQUE;
    if (str == "hamilton") return AlgorithmType::HAMILTON_CIRCUIT;
    if (str == "scc") return AlgorithmType::SCC;
    throw std::invalid_argument("Unknown algorithm: " + str);
}

std::string AlgorithmFactory::typeToString(AlgorithmType type) {
    switch (type) {
        case AlgorithmType::EULER_CIRCUIT: return "euler";
        case AlgorithmType::MST_WEIGHT: return "mst";
        case AlgorithmType::MAX_CLIQUE: return "clique";
        case AlgorithmType::HAMILTON_CIRCUIT: return "hamilton";
        case AlgorithmType::SCC: return "scc";
        default: return "unknown";
    }
}