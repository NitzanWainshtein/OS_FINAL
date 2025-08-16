#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

void printExamples() {
    std::cout << "\n=== Example Graphs to Try ===" << std::endl;
    std::cout << "Triangle (HAS Euler):    3 3 0-1 1-2 2-0" << std::endl;
    std::cout << "Square (HAS Euler):      4 4 0-1 1-2 2-3 3-0" << std::endl;
    std::cout << "Path (NO Euler):         3 2 0-1 1-2" << std::endl;
    std::cout << "Complete K4 (NO Euler):  4 6 0-1 0-2 0-3 1-2 1-3 2-3" << std::endl;
    std::cout << "Single vertex:           1 0" << std::endl;
    std::cout << "Two connected:           2 1 0-1" << std::endl;
    std::cout << "=============================" << std::endl;
}

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cout << "Connection failed! Is server running?" << std::endl;
        std::cout << "Start server with: ./bin/tcp_server" << std::endl;
        return -1;
    }
    
    std::cout << "Connected to Euler server!" << std::endl;
    printExamples();
    
    // Read welcome
    char buffer[1024];
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << buffer;
    
    // Interactive mode
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        
        if (input == "quit" || input == "exit") {
            send(clientSocket, input.c_str(), input.length(), 0);
            break;
        }
        
        if (input == "help" || input == "examples") {
            printExamples();
            continue;
        }
        
        send(clientSocket, input.c_str(), input.length(), 0);
        
        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << buffer;
    }
    
    close(clientSocket);
    return 0;
}