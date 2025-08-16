#include "MaxCliqueAlgorithm.h"
#include <algorithm>
#include <sstream>

std::string MaxCliqueAlgorithm::execute(const Graph& graph) {
    std::vector<int> maxClique;
    std::vector<int> current;
    std::vector<int> candidates;
    
    for (int i = 0; i < graph.getVertexCount(); ++i) {
        candidates.push_back(i);
    }
    
    findMaxClique(graph, current, candidates, maxClique, 0);
    
    std::stringstream ss;
    ss << "Max clique size: " << maxClique.size() << ", vertices: ";
    for (int vertex : maxClique) {
        ss << vertex << " ";
    }
    return ss.str();
}

void MaxCliqueAlgorithm::findMaxClique(const Graph& graph, std::vector<int>& current,
                                      std::vector<int>& candidates, std::vector<int>& maxClique, int start) {
    if (current.size() > maxClique.size()) {
        maxClique = current;
    }
    
    for (int i = start; i < candidates.size(); ++i) {
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
            auto neighbors = graph.getNeighbors(vertices[i]);
            if (std::find(neighbors.begin(), neighbors.end(), vertices[j]) == neighbors.end()) {
                return false;
            }
        }
    }
    return true;
}