#include "EulerAlgorithm.h"
#include <sstream>

std::string EulerAlgorithm::execute(const Graph& graph) {
    Graph workingGraph = graph;
    
    if (!workingGraph.hasEulerCircuit()) {
        return "No Euler circuit exists";
    }
    
    auto circuit = workingGraph.findEulerCircuit();
    std::stringstream ss;
    ss << "Euler circuit: ";
    for (size_t i = 0; i < circuit.size(); ++i) {
        ss << circuit[i];
        if (i < circuit.size() - 1) ss << " -> ";
    }
    return ss.str();
}