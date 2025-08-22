#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

class LFClient {
private:
    int socket_fd;
    bool connected;
    
public:
    LFClient() : socket_fd(-1), connected(false) {}
    
    ~LFClient() {
        disconnect();
    }
    
    bool connect(const std::string& host, int port) {
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd < 0) {
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }
        
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        
        if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0) {
            std::cerr << "Invalid address: " << host << std::endl;
            close(socket_fd);
            return false;
        }
        
        if (::connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Connection failed to " << host << ":" << port << std::endl;
            std::cerr << "Make sure server is running: ../bin/q8_server" << std::endl;
            close(socket_fd);
            return false;
        }
        
        connected = true;
        std::cout << "✓ Connected to Leader-Follower server at " << host << ":" << port << std::endl;
        return true;
    }
    
    void disconnect() {
        if (connected && socket_fd >= 0) {
            close(socket_fd);
            connected = false;
        }
    }
    
    void runInteractive() {
        if (!connected) return;
        
        // Read welcome
        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));
        recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
        std::cout << buffer;
        
        showExamples();
        
        std::string input;
        while (connected) {
            std::getline(std::cin, input);
            
            if (input == "quit" || input == "exit") {
                send(socket_fd, input.c_str(), input.length(), 0);
                break;
            }
            
            if (input == "examples") {
                showExamples();
                continue;
            }
            
            if (input.empty()) {
                std::cout << "Enter graph or 'examples' for help\n> ";
                continue;
            }
            
            // Send to server
            send(socket_fd, input.c_str(), input.length(), 0);
            
            // Get response
            memset(buffer, 0, sizeof(buffer));
            int bytes = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
            
            if (bytes <= 0) {
                std::cout << "Connection lost" << std::endl;
                break;
            }
            
            std::cout << buffer;
        }
    }
    
private:
    void showExamples() {
        std::cout << "\n=== Leader-Follower Examples ===" << std::endl;
        std::cout << "Format: vertices edges edge1 edge2 ..." << std::endl;
        std::cout << "ALL algorithms will be executed automatically!" << std::endl;
        std::cout << std::endl;
        std::cout << "Good test cases:" << std::endl;
        std::cout << "  3 3 0-1 1-2 2-0          (triangle - most algorithms work)" << std::endl;
        std::cout << "  4 4 0-1 1-2 2-3 3-0      (square - good for all)" << std::endl;
        std::cout << "  4 6 0-1 0-2 0-3 1-2 1-3 2-3  (K4 complete)" << std::endl;
        std::cout << "  5 5 0-1 1-2 2-3 3-4 4-0  (pentagon cycle)" << std::endl;
        std::cout << std::endl;
        std::cout << "Algorithms executed:" << std::endl;
        std::cout << "  1. Euler Circuit (undirected)" << std::endl;
        std::cout << "  2. Max Clique (undirected)" << std::endl;
        std::cout << "  3. MST Weight (undirected)" << std::endl;
        std::cout << "  4. Hamilton Circuit (undirected)" << std::endl;
        std::cout << "  5. SCC (directed version of same graph)" << std::endl;
        std::cout << std::endl;
        std::cout << "Commands: help, stats, quit, examples" << std::endl;
        std::cout << "================================" << std::endl;
        std::cout << "> ";
    }
};

int main(int argc, char* argv[]) {
    std::string host = "127.0.0.1";
    int port = 8080;
    
    // Parse arguments
    if (argc > 3) {
        std::cerr << "Usage: " << argv[0] << " [host] [port]" << std::endl;
        return 1;
    }
    
    if (argc >= 2) {
        host = argv[1];
    }
    if (argc == 3) {
        try {
            port = std::stoi(argv[2]);
        } catch (const std::exception& e) {
            std::cerr << "Invalid port: " << argv[2] << std::endl;
            return 1;
        }
    }
    
    std::cout << "=== Q8: Leader-Follower Client ===" << std::endl;
    std::cout << "Connecting to " << host << ":" << port << "..." << std::endl;
    
    LFClient client;
    if (!client.connect(host, port)) {
        return 1;
    }
    
    std::cout << "Each graph will be processed by ALL algorithms using Leader-Follower pattern" << std::endl;
    std::cout << "Type graphs or 'examples' for help" << std::endl;
    std::cout << "Press Ctrl+C to exit" << std::endl;
    
    client.runInteractive();
    
    return 0;
}