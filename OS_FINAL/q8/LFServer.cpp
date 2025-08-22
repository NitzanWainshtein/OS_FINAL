#include "LFServer.h"
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <iomanip>

LFServer::LFServer(int serverPort, size_t numWorkers) 
    : port(serverPort), running(false), serverSocket(-1), clientCounter(0),
      waitingFollowers(0), totalRequestsProcessed(0), leaderPromotions(0), 
      totalProcessingTime(0) {
    
    // Create worker threads for Leader-Follower pattern
    for (size_t i = 0; i < numWorkers; ++i) {
        workerThreads.emplace_back(&LFServer::workerThread, this);
    }
    
    log("Leader-Follower server created with " + std::to_string(numWorkers) + " worker threads");
}

LFServer::~LFServer() {
    stop();
}

void LFServer::start() {
    if (running) {
        log("Server already running");
        return;
    }
    
    try {
        setupSocket();
        running = true;
        
        log("=== Leader-Follower Server Started ===");
        log("Port: " + std::to_string(port));
        log("Worker threads: " + std::to_string(workerThreads.size()));
        log("Protocol: Multi-algorithm requests");
        log("Pattern: Leader-Follower threading");
        log("=====================================");
        
        // Start accepting clients
        serverThread = std::thread(&LFServer::acceptClients, this);
        
    } catch (const std::exception& e) {
        log("Failed to start server: " + std::string(e.what()));
        stop();
        throw;
    }
}

void LFServer::stop() {
    if (!running) return;
    
    running = false;
    
    // Close server socket
    if (serverSocket >= 0) {
        close(serverSocket);
        serverSocket = -1;
    }
    
    // Wake up all worker threads
    {
        std::lock_guard<std::mutex> lock(queueMutex);
    }
    leaderCondition.notify_all();
    followerCondition.notify_all();
    
    // Join server thread
    if (serverThread.joinable()) {
        serverThread.join();
    }
    
    // Join worker threads
    for (auto& worker : workerThreads) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    
    log("=== Leader-Follower Server Stopped ===");
}

void LFServer::setupSocket() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw std::runtime_error("Failed to create socket");
    }
    
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        close(serverSocket);
        throw std::runtime_error("Failed to set socket options");
    }
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    if (bind(serverSocket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        close(serverSocket);
        throw std::runtime_error("Failed to bind to port " + std::to_string(port));
    }
    
    if (listen(serverSocket, 10) < 0) {
        close(serverSocket);
        throw std::runtime_error("Failed to listen on socket");
    }
}

void LFServer::acceptClients() {
    while (running) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            if (running) {
                log("Failed to accept client connection");
            }
            continue;
        }
        
        int clientId = ++clientCounter;
        std::string clientIP = inet_ntoa(clientAddr.sin_addr);
        logRequest(clientId, "Connected from " + clientIP);
        
        // Handle client in separate thread (not part of LF pool)
        std::thread clientThread(&LFServer::handleNewClient, this, clientSocket, clientId);
        clientThread.detach();
    }
}

void LFServer::handleNewClient(int clientSocket, int clientId) {
    try {
        // Send welcome message
        std::string welcome = 
            "=== Leader-Follower Algorithms Server ===\n"
            "Client ID: " + std::to_string(clientId) + "\n"
            "Format: vertices edges edge1 edge2 ...\n"
            "All algorithms will be executed: euler, clique, mst, hamilton, scc\n"
            "\n"
            "Examples:\n"
            "  3 3 0-1 1-2 2-0    (triangle - works for most algorithms)\n"
            "  4 4 0-1 1-2 2-3 3-0  (square - good test case)\n"
            "\n"
            "Commands: help, stats, quit\n"
            "> ";
        
        send(clientSocket, welcome.c_str(), welcome.length(), 0);
        
        char buffer[2048];
        while (running) {
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            
            if (bytesRead <= 0) {
                logRequest(clientId, "Disconnected");
                break;
            }
            
            std::string input(buffer);
            input.erase(input.find_last_not_of("\r\n") + 1);
            
            logRequest(clientId, "Request: " + input);
            
            if (input == "quit" || input == "exit") {
                std::string response = "Goodbye!\n";
                send(clientSocket, response.c_str(), response.length(), 0);
                break;
            } else if (input == "help") {
                send(clientSocket, welcome.c_str(), welcome.length(), 0);
                continue;
            } else if (input == "stats") {
                std::ostringstream stats;
                stats << "\n=== Server Statistics ===\n";
                stats << "Requests processed: " << totalRequestsProcessed << "\n";
                stats << "Leader promotions: " << leaderPromotions << "\n";
                stats << "Worker threads: " << workerThreads.size() << "\n";
                stats << "========================\n> ";
                std::string statsStr = stats.str();
                send(clientSocket, statsStr.c_str(), statsStr.length(), 0);
                continue;
            } else if (input.empty()) {
                std::string response = "Empty input. Type 'help' for usage.\n> ";
                send(clientSocket, response.c_str(), response.length(), 0);
                continue;
            }
            
            // Create request and add to Leader-Follower queue
            auto request = std::make_shared<ClientRequest>(clientId, clientSocket, input);
            
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                requestQueue.push(request);
            }
            
            // Wake up the leader to process request
            leaderCondition.notify_one();
            
            logRequest(clientId, "Request queued for LF processing");
        }
        
    } catch (const std::exception& e) {
        logRequest(clientId, "Error: " + std::string(e.what()));
    }
    
    close(clientSocket);
}

void LFServer::workerThread() {
    std::thread::id myId = std::this_thread::get_id();
    
    while (running) {
        std::shared_ptr<ClientRequest> request;
        bool hasRequest = false;
        
        // Leader-Follower logic
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            
            // Try to become leader if no leader exists
            if (currentLeader == std::thread::id{} && becomeLeader()) {
                // I'm now the leader
                leaderPromotions++;
                
                // Wait for a request
                leaderCondition.wait(lock, [this] {
                    return !running || !requestQueue.empty();
                });
                
                if (!running) break;
                
                if (!requestQueue.empty()) {
                    request = requestQueue.front();
                    requestQueue.pop();
                    hasRequest = true;
                    
                    // Give up leadership
                    {
                        std::lock_guard<std::mutex> leaderLock(leaderMutex);
                        currentLeader = std::thread::id{};
                    }
                    
                    // Promote new leader
                    promoteNewLeader();
                }
            } else {
                // I'm a follower
                waitingFollowers++;
                
                // Wait to be promoted to leader
                followerCondition.wait(lock, [this, myId] {
                    return !running || currentLeader == myId;
                });
                
                waitingFollowers--;
                
                if (!running) break;
                
                // If promoted to leader, continue to next iteration
                continue;
            }
        }
        
        // Process request outside of lock
        if (hasRequest) {
            processRequest(request);
        }
    }
}

bool LFServer::becomeLeader() {
    std::lock_guard<std::mutex> lock(leaderMutex);
    if (currentLeader == std::thread::id{}) {
        currentLeader = std::this_thread::get_id();
        return true;
    }
    return false;
}

void LFServer::promoteNewLeader() {
    if (waitingFollowers > 0) {
        // Promote one follower to leader
        {
            std::lock_guard<std::mutex> lock(leaderMutex);
            // The promoted thread will set itself as leader
        }
        followerCondition.notify_one();
    }
}

void LFServer::processRequest(std::shared_ptr<ClientRequest> request) {
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        logRequest(request->clientId, "Processing by thread " + 
                  std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id()) % 1000));
        
        std::string response = parseAndExecuteAlgorithms(request->requestData, request->clientId);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        totalProcessingTime += duration.count();
        
        response += "\n> ";
        send(request->clientSocket, response.c_str(), response.length(), 0);
        
        totalRequestsProcessed++;
        logRequest(request->clientId, "Completed in " + std::to_string(duration.count()) + "μs");
        
    } catch (const std::exception& e) {
        std::string errorResponse = formatError(e.what(), request->clientId) + "\n> ";
        send(request->clientSocket, errorResponse.c_str(), errorResponse.length(), 0);
        logRequest(request->clientId, "Error: " + std::string(e.what()));
    }
}

std::string LFServer::parseAndExecuteAlgorithms(const std::string& input, int clientId) {
    // Parse graph
    std::istringstream iss(input);
    int vertices, edges;
    
    if (!(iss >> vertices)) {
        throw std::invalid_argument("Missing vertices count");
    }
    if (!(iss >> edges)) {
        throw std::invalid_argument("Missing edges count");
    }
    
    if (vertices <= 0 || vertices > 50) {
        throw std::invalid_argument("Vertices must be 1-50");
    }
    
    // Parse edges into undirected graph first
    Graph undirectedGraph(vertices, false);
    std::vector<std::pair<int, int>> edgeList;
    
    std::string edgeStr;
    int edgeCount = 0;
    
    while (iss >> edgeStr && edgeCount < edges) {
        size_t dashPos = edgeStr.find('-');
        if (dashPos == std::string::npos) {
            throw std::invalid_argument("Invalid edge: " + edgeStr);
        }
        
        int u = std::stoi(edgeStr.substr(0, dashPos));
        int v = std::stoi(edgeStr.substr(dashPos + 1));
        
        if (u < 0 || u >= vertices || v < 0 || v >= vertices || u == v) {
            throw std::invalid_argument("Invalid edge: " + edgeStr);
        }
        
        undirectedGraph.addEdge(u, v);
        edgeList.emplace_back(u, v);
        edgeCount++;
    }
    
    if (edgeCount != edges) {
        throw std::invalid_argument("Edge count mismatch");
    }
    
    // Create directed graph for SCC
    Graph directedGraph(vertices, true);
    for (const auto& edge : edgeList) {
        directedGraph.addEdge(edge.first, edge.second);
    }
    
    // Execute ALL algorithms
    std::vector<AlgorithmResult> results;
    auto start = std::chrono::high_resolution_clock::now();
    
    // Algorithm 1: Euler Circuit (undirected)
    try {
        auto eulerAlg = AlgorithmFactory::createAlgorithm(AlgorithmType::EULER_CIRCUIT);
        results.push_back(eulerAlg->execute(undirectedGraph));
    } catch (const std::exception& e) {
        AlgorithmResult errorResult(AlgorithmType::EULER_CIRCUIT);
        errorResult.success = false;
        errorResult.error = e.what();
        results.push_back(errorResult);
    }
    
    // Algorithm 2: Max Clique (undirected, if small enough)
    try {
        auto cliqueAlg = AlgorithmFactory::createAlgorithm(AlgorithmType::MAX_CLIQUE);
        results.push_back(cliqueAlg->execute(undirectedGraph));
    } catch (const std::exception& e) {
        AlgorithmResult errorResult(AlgorithmType::MAX_CLIQUE);
        errorResult.success = false;
        errorResult.error = e.what();
        results.push_back(errorResult);
    }
    
    // Algorithm 3: MST (undirected)
    try {
        auto mstAlg = AlgorithmFactory::createAlgorithm(AlgorithmType::MST_WEIGHT);
        results.push_back(mstAlg->execute(undirectedGraph));
    } catch (const std::exception& e) {
        AlgorithmResult errorResult(AlgorithmType::MST_WEIGHT);
        errorResult.success = false;
        errorResult.error = e.what();
        results.push_back(errorResult);
    }
    
    // Algorithm 4: Hamilton Circuit (undirected, if small enough)
    try {
        auto hamiltonAlg = AlgorithmFactory::createAlgorithm(AlgorithmType::HAMILTON_CIRCUIT);
        results.push_back(hamiltonAlg->execute(undirectedGraph));
    } catch (const std::exception& e) {
        AlgorithmResult errorResult(AlgorithmType::HAMILTON_CIRCUIT);
        errorResult.success = false;
        errorResult.error = e.what();
        results.push_back(errorResult);
    }
    
    // Algorithm 5: SCC (directed)
    try {
        auto sccAlg = AlgorithmFactory::createAlgorithm(AlgorithmType::SCC);
        results.push_back(sccAlg->execute(directedGraph));
    } catch (const std::exception& e) {
        AlgorithmResult errorResult(AlgorithmType::SCC);
        errorResult.success = false;
        errorResult.error = e.what();
        results.push_back(errorResult);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    return formatResponse(results, clientId, duration.count());
}

std::string LFServer::formatResponse(const std::vector<AlgorithmResult>& results, 
                                   int clientId, long long totalTime) {
    std::ostringstream response;
    
    response << "\n=== MULTI-ALGORITHM ANALYSIS ===\n";
    response << "Client: " << clientId << "\n";
    response << "Thread: " << std::hash<std::thread::id>{}(std::this_thread::get_id()) % 1000 << "\n";
    response << "Algorithms executed: " << results.size() << "\n";
    response << "Total time: " << totalTime << "ms\n";
    response << "\n";
    
    for (const auto& result : results) {
        response << "• " << AlgorithmFactory::typeToString(result.type) << ": ";
        if (result.success) {
            response << result.result;
            if (!result.executionTime.empty()) {
                response << " (" << result.executionTime << ")";
            }
        } else {
            response << "ERROR - " << result.error;
        }
        response << "\n";
    }
    
    response << "===============================";
    return response.str();
}

std::string LFServer::formatError(const std::string& error, int clientId) {
    std::ostringstream response;
    response << "\n=== ERROR ===\n";
    if (clientId >= 0) {
        response << "Client: " << clientId << "\n";
    }
    response << "Error: " << error << "\n";
    response << "=============";
    return response.str();
}

void LFServer::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    std::cout << std::put_time(&tm, "[%H:%M:%S] ") << message << std::endl;
}

void LFServer::logRequest(int clientId, const std::string& activity) {
    log("Client " + std::to_string(clientId) + ": " + activity);
}

void LFServer::printStatistics() const {
    std::cout << "\n=== Leader-Follower Server Statistics ===" << std::endl;
    std::cout << "Total requests processed: " << totalRequestsProcessed << std::endl;
    std::cout << "Leader promotions: " << leaderPromotions << std::endl;
    std::cout << "Worker threads: " << workerThreads.size() << std::endl;
    std::cout << "Average processing time: ";
    if (totalRequestsProcessed > 0) {
        std::cout << (totalProcessingTime / totalRequestsProcessed) << "μs" << std::endl;
    } else {
        std::cout << "N/A" << std::endl;
    }
    std::cout << "=========================================" << std::endl;
}