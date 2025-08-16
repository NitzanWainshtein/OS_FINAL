#ifndef ALGORITHMS_SERVER_H
#define ALGORITHMS_SERVER_H

#include "../algorithms/AlgorithmFactory.h"
#include "../graph/Graph.h"

struct AlgorithmRequest {
    int clientId;
    AlgorithmType algorithmType;
    Graph graph;
    
    AlgorithmRequest(int id, AlgorithmType type, const Graph& g) 
        : clientId(id), algorithmType(type), graph(g) {}
};

struct AlgorithmResult {
    int clientId;
    AlgorithmType algorithmType;
    std::string result;
    bool success;
    std::string error;
    
    AlgorithmResult(int id, AlgorithmType type) 
        : clientId(id), algorithmType(type), success(false) {}
};

class AlgorithmsServer {
private:
    int port;
    bool running;
    
public:
    AlgorithmsServer(int serverPort);
    void start();
    void stop();
    AlgorithmResult processRequest(const AlgorithmRequest& request);
};

#endif