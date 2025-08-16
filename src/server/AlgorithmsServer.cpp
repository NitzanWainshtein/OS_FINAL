#include "AlgorithmsServer.h"
#include <iostream>

AlgorithmsServer::AlgorithmsServer(int serverPort) : port(serverPort), running(false) {
}

void AlgorithmsServer::start() {
    running = true;
    std::cout << "Algorithms Server started on port " << port << std::endl;
    std::cout << "Supported algorithms: euler, mst, clique, hamilton, scc" << std::endl;
}

void AlgorithmsServer::stop() {
    running = false;
    std::cout << "Algorithms Server stopped" << std::endl;
}

AlgorithmResult AlgorithmsServer::processRequest(const AlgorithmRequest& request) {
    AlgorithmResult result(request.clientId, request.algorithmType);
    
    std::cout << "Processing " << AlgorithmFactory::typeToString(request.algorithmType) 
              << " request from client " << request.clientId << std::endl;
    
    try {
        auto algorithm = AlgorithmFactory::createAlgorithm(request.algorithmType);
        result.result = algorithm->execute(request.graph);
        result.success = true;
        
        std::cout << "Result: " << result.result << std::endl;
        
    } catch (const std::exception& e) {
        result.error = e.what();
        result.success = false;
        std::cout << "Error: " << result.error << std::endl;
    }
    
    return result;
}