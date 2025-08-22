#include "EulerAlgorithm.h"
#include <sstream>
#include <chrono>

AlgorithmResult EulerAlgorithm::execute(const Graph& graph) {
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        Graph workingGraph = graph;
        
        if (!workingGraph.hasEulerCircuit()) {
            std::string reason;
            if (!workingGraph.isConnected()) {
                reason = "graph not connected";
            } else if (!graph.isDirected()) {
                reason = "not all vertices have even degree";
            } else {
                reason = "in-degree ≠ out-degree for some vertices";
            }
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
            AlgorithmResult result = createResult(AlgorithmType::EULER_CIRCUIT, 
                "No Euler circuit (" + reason + ")");
            result.executionTime = std::to_string(duration.count()) + "μs";
            return result;
        }
        
        auto circuit = workingGraph.findEulerCircuit();
        
        std::ostringstream ss;
        ss << "Euler circuit: ";
        for (size_t i = 0; i < circuit.size(); ++i) {
            ss << circuit[i];
            if (i < circuit.size() - 1) ss << "→";
        }
        ss << " (length: " << circuit.size() << ")";
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        AlgorithmResult result = createResult(AlgorithmType::EULER_CIRCUIT, ss.str());
        result.executionTime = std::to_string(duration.count()) + "μs";
        return result;
        
    } catch (const std::exception& e) {
        return createError(AlgorithmType::EULER_CIRCUIT, e.what());
    }
}