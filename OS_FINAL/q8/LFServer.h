#ifndef LF_SERVER_H
#define LF_SERVER_H

#include "../common/Graph.h"
#include "../algorithms/AlgorithmFactory.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <functional>
#include <vector>
#include <memory>

struct ClientRequest {
    int clientId;
    int clientSocket;
    std::string requestData;
    std::chrono::steady_clock::time_point timestamp;
    
    ClientRequest(int id, int socket, const std::string& data) 
        : clientId(id), clientSocket(socket), requestData(data),
          timestamp(std::chrono::steady_clock::now()) {}
};

class LFServer {
private:
    // Server basics
    int port;
    std::atomic<bool> running;
    int serverSocket;
    std::thread serverThread;
    int clientCounter;
    
    // Leader-Follower pattern
    std::vector<std::thread> workerThreads;
    std::queue<std::shared_ptr<ClientRequest>> requestQueue;
    std::mutex queueMutex;
    std::condition_variable leaderCondition;
    std::condition_variable followerCondition;
    
    std::atomic<int> waitingFollowers;
    std::thread::id currentLeader;
    std::mutex leaderMutex;
    
    // Statistics
    std::atomic<long long> totalRequestsProcessed;
    std::atomic<long long> leaderPromotions;
    std::atomic<long long> totalProcessingTime; // microseconds
    std::mutex logMutex;
    
public:
    LFServer(int serverPort, size_t numWorkers = 4);
    ~LFServer();
    
    void start();
    void stop();
    bool isRunning() const { return running; }
    
    // Statistics
    void printStatistics() const;
    
private:
    // Server management
    void setupSocket();
    void acceptClients();
    void handleNewClient(int clientSocket, int clientId);
    
    // Leader-Follower implementation
    void workerThread();
    void promoteNewLeader();
    bool becomeLeader();
    void processRequest(std::shared_ptr<ClientRequest> request);
    
    // Request processing
    std::string parseAndExecuteAlgorithms(const std::string& input, int clientId);
    Graph parseGraph(const std::string& input, const std::string& algorithmName);
    std::string formatResponse(const std::vector<AlgorithmResult>& results, int clientId, 
                             long long totalTime);
    std::string formatError(const std::string& error, int clientId = -1);
    
    // Utilities
    void log(const std::string& message);
    void logRequest(int clientId, const std::string& activity);
};

#endif