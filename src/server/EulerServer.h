#ifndef EULER_SERVER_H
#define EULER_SERVER_H

#include "../graph/Graph.h"
#include <string>
#include <vector>
#include <thread>
#include <atomic>

struct GraphRequest {
    int clientId;
    Graph graph;
    
    GraphRequest(int id, const Graph& g) : clientId(id), graph(g) {}
};

class EulerServer {
public:
    struct EulerResult {
        int clientId;
        bool hasEulerCircle;
        std::vector<int> eulerCircle;
        std::string message;
        
        EulerResult(int id) : clientId(id), hasEulerCircle(false) {}
    };

private:
    int port;
    bool running;
    int serverSocket;
    std::thread serverThread;
    
public:
    EulerServer(int serverPort);
    ~EulerServer();
    
    void start();
    void stop();
    
    // Old simulation method (for compatibility)
    EulerResult processRequest(const GraphRequest& request);
    
    // New TCP methods
    void startTcpServer();
    void handleClient(int clientSocket);
    std::string processEulerRequestTcp(const std::string& input);
};

#endif