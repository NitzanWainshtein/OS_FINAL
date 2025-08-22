#include "MaxCliqueAlgorithm.h"
#include <algorithm>
#include <sstream>
#include <chrono>

AlgorithmResult MaxCliqueAlgorithm::execute(const Graph& graph) {
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        validateGraph(graph, false, true);
        
        if (graph.getVertexCount() > 15) {
            return createError(AlgorithmType::MAX_CLIQUE, 
                "Graph too large for exact algorithm (max 15 vertices)");
        }
        
        std::vector<int> maxClique;
        std::vector<int> current;
        std::vector<int> candidates;
        
        for (int i = 0; i < graph.getVertexCount(); ++i) {
            candidates.push_back(i);
        }
        
        findMaxClique(graph, current, candidates, maxClique, 0);
        
        std::ostringstream ss;
        ss << "Max clique size: " << maxClique.size();
        if (!maxClique.empty()) {
            ss << ", vertices: {";
            for (size_t i = 0; i < maxClique.size(); ++i) {
                ss << maxClique[i];
                if (i < maxClique.size() - 1) ss << ",";
            }
            ss << "}";
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        AlgorithmResult result = createResult(AlgorithmType::MAX_CLIQUE, ss.str());
        result.executionTime = std::to_string(duration.count()) + "μs";
        return result;
        
    } catch (const std::exception& e) {
        return createError(AlgorithmType::MAX_CLIQUE, e.what());
    }
}

void MaxCliqueAlgorithm::findMaxClique(const Graph& graph, std::vector<int>& current,
                                      std::vector<int>& candidates, std::vector<int>& maxClique, int start) {
    if (current.size() > maxClique.size()) {
        maxClique = current;
    }
    
    for (int i = start; i < (int)candidates.size(); ++i) {
        int vertex = candidates[i];
        current.push_back(vertex);
        
        if (areAllConnected(graph, current)) {
            findMaxClique(graph, current, candidates, maxClique, i + 1);
        }
        
        current.pop_back();
    }
}

bool MaxCliqueAlgorithm::areAllConnected(const Graph& graph, const std::vector<int>& vertices) {
    for (size_t i = 0; i < vertices.size(); ++i) {
        for (size_t j = i + 1; j < vertices.size(); ++j) {
            if (!graph.hasEdge(vertices[i], vertices[j])) {
                return false;
            }
        }
    }
    return true;
}