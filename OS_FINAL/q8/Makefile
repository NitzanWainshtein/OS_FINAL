CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -O0 -pthread
INCLUDES = -I../common -I../algorithms -I../threading

# Directories
COMMON_DIR = ../common
ALGORITHMS_DIR = ../algorithms
THREADING_DIR = ../threading
OBJ_DIR = ../obj
BIN_DIR = ../bin

# Targets
TARGET_SERVER = $(BIN_DIR)/q8_server
TARGET_CLIENT = $(BIN_DIR)/q8_client

# Sources
SERVER_SOURCES = server_main.cpp LFServer.cpp $(COMMON_DIR)/Graph.cpp \
                $(ALGORITHMS_DIR)/AlgorithmFactory.cpp \
                $(ALGORITHMS_DIR)/EulerAlgorithm.cpp \
                $(ALGORITHMS_DIR)/MaxCliqueAlgorithm.cpp \
                $(ALGORITHMS_DIR)/MSTAlgorithm.cpp \
                $(ALGORITHMS_DIR)/HamiltonAlgorithm.cpp \
                $(ALGORITHMS_DIR)/SCCAlgorithm.cpp

CLIENT_SOURCES = client.cpp

# Objects
SERVER_OBJECTS = $(OBJ_DIR)/q8_server_main.o $(OBJ_DIR)/q8_LFServer.o \
                $(OBJ_DIR)/Graph.o $(OBJ_DIR)/AlgorithmFactory.o \
                $(OBJ_DIR)/EulerAlgorithm.o $(OBJ_DIR)/MaxCliqueAlgorithm.o \
                $(OBJ_DIR)/MSTAlgorithm.o $(OBJ_DIR)/HamiltonAlgorithm.o \
                $(OBJ_DIR)/SCCAlgorithm.o

CLIENT_OBJECTS = $(OBJ_DIR)/q8_client.o

# Create directories
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

all: server client

server: $(TARGET_SERVER)

client: $(TARGET_CLIENT)

$(TARGET_SERVER): $(SERVER_OBJECTS)
	$(CXX) $(SERVER_OBJECTS) -o $(TARGET_SERVER) -pthread
	@echo "✓ Q8 Leader-Follower server: $(TARGET_SERVER)"

$(TARGET_CLIENT): $(CLIENT_OBJECTS)
	$(CXX) $(CLIENT_OBJECTS) -o $(TARGET_CLIENT)
	@echo "✓ Q8 client: $(TARGET_CLIENT)"

# Server objects
$(OBJ_DIR)/q8_server_main.o: server_main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c server_main.cpp -o $(OBJ_DIR)/q8_server_main.o

$(OBJ_DIR)/q8_LFServer.o: LFServer.cpp LFServer.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c LFServer.cpp -o $(OBJ_DIR)/q8_LFServer.o

$(OBJ_DIR)/q8_client.o: client.cpp
	$(CXX) $(CXXFLAGS) -c client.cpp -o $(OBJ_DIR)/q8_client.o

# Shared objects (reuse from previous questions)
$(OBJ_DIR)/Graph.o: $(COMMON_DIR)/Graph.cpp $(COMMON_DIR)/Graph.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(COMMON_DIR)/Graph.cpp -o $(OBJ_DIR)/Graph.o

$(OBJ_DIR)/AlgorithmFactory.o: $(ALGORITHMS_DIR)/AlgorithmFactory.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(ALGORITHMS_DIR)/AlgorithmFactory.cpp -o $(OBJ_DIR)/AlgorithmFactory.o

$(OBJ_DIR)/EulerAlgorithm.o: $(ALGORITHMS_DIR)/EulerAlgorithm.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(ALGORITHMS_DIR)/EulerAlgorithm.cpp -o $(OBJ_DIR)/EulerAlgorithm.o

$(OBJ_DIR)/MaxCliqueAlgorithm.o: $(ALGORITHMS_DIR)/MaxCliqueAlgorithm.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(ALGORITHMS_DIR)/MaxCliqueAlgorithm.cpp -o $(OBJ_DIR)/MaxCliqueAlgorithm.o

$(OBJ_DIR)/MSTAlgorithm.o: $(ALGORITHMS_DIR)/MSTAlgorithm.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(ALGORITHMS_DIR)/MSTAlgorithm.cpp -o $(OBJ_DIR)/MSTAlgorithm.o

$(OBJ_DIR)/HamiltonAlgorithm.o: $(ALGORITHMS_DIR)/HamiltonAlgorithm.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(ALGORITHMS_DIR)/HamiltonAlgorithm.cpp -o $(OBJ_DIR)/HamiltonAlgorithm.o

$(OBJ_DIR)/SCCAlgorithm.o: $(ALGORITHMS_DIR)/SCCAlgorithm.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(ALGORITHMS_DIR)/SCCAlgorithm.cpp -o $(OBJ_DIR)/SCCAlgorithm.o

clean:
	rm -f $(SERVER_OBJECTS) $(CLIENT_OBJECTS) $(TARGET_SERVER) $(TARGET_CLIENT)
	@echo "✓ Q8 cleaned"

test: all
	@echo "=== Q8 Leader-Follower Test ==="
	@echo "This demonstrates the Leader-Follower threading pattern"
	@echo ""
	@echo "1. Terminal 1: $(TARGET_SERVER) [port] [workers]"
	@echo "2. Terminal 2: $(TARGET_CLIENT)"
	@echo "3. Send graphs - ALL algorithms execute automatically"
	@echo ""
	@echo "The server uses Leader-Follower pattern where:"
	@echo "- One thread is the 'leader' waiting for requests"
	@echo "- Other threads are 'followers' waiting to be promoted"
	@echo "- When leader gets request, it processes it and promotes new leader"

run-server: server
	@echo "Starting Leader-Follower server with 4 workers..."
	$(TARGET_SERVER)

run-server-8: server
	@echo "Starting Leader-Follower server with 8 workers..."
	$(TARGET_SERVER) 8080 8

run-client: client
	$(TARGET_CLIENT)

demo: server client
	@echo "=== Q8 Demo Instructions ==="
	@echo ""
	@echo "1. Run server: make run-server"
	@echo "2. Run client: make run-client" 
	@echo "3. Try these graphs:"
	@echo "   3 3 0-1 1-2 2-0"
	@echo "   4 4 0-1 1-2 2-3 3-0"
	@echo "   4 6 0-1 0-2 0-3 1-2 1-3 2-3"
	@echo ""
	@echo "Watch server logs to see Leader-Follower pattern in action!"
	@echo "Each request processes ALL 5 algorithms automatically."

install: all
	@echo "✓ Q8 Leader-Follower ready:"
	@echo "  Server: $(TARGET_SERVER) [port] [workers]"
	@echo "  Client: $(TARGET_CLIENT) [host] [port]"
	@echo ""
	@echo "Features:"
	@echo "- Leader-Follower threading pattern"
	@echo "- Executes ALL algorithms on each graph"
	@echo "- Thread statistics and performance monitoring"
	@echo "- Configurable number of worker threads"

.PHONY: all server client clean test run-server run-server-8 run-client demo install

# Dependencies
$(OBJ_DIR)/q8_server_main.o: LFServer.h
$(OBJ_DIR)/q8_LFServer.o: LFServer.h $(COMMON_DIR)/Graph.h $(ALGORITHMS_DIR)/AlgorithmFactory.h
$(OBJ_DIR)/Graph.o: $(COMMON_DIR)/Graph.h
$(OBJ_DIR)/AlgorithmFactory.o: $(ALGORITHMS_DIR)/AlgorithmStrategy.h
$(OBJ_DIR)/EulerAlgorithm.o: $(ALGORITHMS_DIR)/AlgorithmStrategy.h
$(OBJ_DIR)/MaxCliqueAlgorithm.o: $(ALGORITHMS_DIR)/AlgorithmStrategy.h
$(OBJ_DIR)/MSTAlgorithm.o: $(ALGORITHMS_DIR)/AlgorithmStrategy.h
$(OBJ_DIR)/HamiltonAlgorithm.o: $(ALGORITHMS_DIR)/AlgorithmStrategy.h
$(OBJ_DIR)/SCCAlgorithm.o: $(ALGORITHMS_DIR)/AlgorithmStrategy.h