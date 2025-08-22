#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

class AlgorithmsClient {
private:
    int socket_fd;
    bool connected;
    
public:
    AlgorithmsClient() : socket_fd(-1), connected(false) {}
    
    ~AlgorithmsClient() {
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
            std::cerr << "Make sure server is running: ../bin/q7_server" << std::endl;
            close(socket_fd);
            return false;
        }
        
        connected = true;
        std::cout << "✓ Connected to " << host << ":" << port << std::endl;
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
                std::cout << "Enter algorithm request or 'examples' for help\n> ";
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
        std::cout << "\n=== Quick Examples ===" << std::endl;
        std::cout << "Format: algorithm vertices edges edge1 edge2 ..." << std::endl;
        std::cout << std::endl;
        std::cout << "EULER (undirected):" << std::endl;
        std::cout << "  euler 3 3 0-1 1-2 2-0" << std::endl;
        std::cout << "  euler 4 4 0-1 1-2 2-3 3-0" << std::endl;
        std::cout << std::endl;
        std::cout << "MAX CLIQUE (undirected):" << std::endl;
        std::cout << "  clique 4 6 0-1 0-2 0-3 1-2 1-3 2-3" << std::endl;
        std::cout << "  clique 3 3 0-1 1-2 2-0" << std::endl;
        std::cout << std::endl;
        std::cout << "MST (undirected):" << std::endl;
        std::cout << "  mst 4 4 0-1 1-2 2-3 3-0" << std::endl;
        std::cout << "  mst 5 6 0-1 0-2 1-3 2-3 3-4 4-1" << std::endl;
        std::cout << std::endl;
        std::cout << "HAMILTON (any):" << std::endl;
        std::cout << "  hamilton 4 4 0-1 1-2 2-3 3-0" << std::endl;
        std::cout << "  hamilton 3 3 0-1 1-2 2-0" << std::endl;
        std::cout << std::endl;
        std::cout << "SCC (directed only):" << std::endl;
        std::cout << "  scc 4 4 0-1 1-2 2-0 0-3" << std::endl;
        std::cout << "  scc 3 3 0-1 1-2 2-0" << std::endl;
        std::cout << std::endl;
        std::cout << "Commands: algorithms, help, quit, examples" << std::endl;
        std::cout << "=======================" << std::endl;
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
    
    std::cout << "=== Q7: Algorithms Client ===" << std::endl;
    std::cout << "Connecting to " << host << ":" << port << "..." << std::endl;
    
    AlgorithmsClient client;
    if (!client.connect(host, port)) {
        return 1;
    }
    
    std::cout << "Type algorithm requests or 'examples' for help" << std::endl;
    std::cout << "Press Ctrl+C to exit" << std::endl;
    
    client.runInteractive();
    
    return 0;
}