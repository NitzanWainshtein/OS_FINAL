CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -O0 -pthread
INCLUDES = -I../common -I../algorithms

# Directories
COMMON_DIR = ../common
ALGORITHMS_DIR = ../algorithms
OBJ_DIR = ../obj
BIN_DIR = ../bin

# Targets
TARGET_SERVER = $(BIN_DIR)/q7_server
TARGET_CLIENT = $(BIN_DIR)/q7_client

# Sources
SERVER_SOURCES = server_main.cpp $(COMMON_DIR)/Graph.cpp $(ALGORITHMS_DIR)/AlgorithmFactory.cpp \
                $(ALGORITHMS_DIR)/EulerAlgorithm.cpp $(ALGORITHMS_DIR)/MaxCliqueAlgorithm.cpp \
                $(ALGORITHMS_DIR)/MSTAlgorithm.cpp $(ALGORITHMS_DIR)/HamiltonAlgorithm.cpp \
                $(ALGORITHMS_DIR)/SCCAlgorithm.cpp

CLIENT_SOURCES = client.cpp

# Objects
SERVER_OBJECTS = $(OBJ_DIR)/q7_server_main.o $(OBJ_DIR)/Graph.o $(OBJ_DIR)/AlgorithmFactory.o \
                $(OBJ_DIR)/EulerAlgorithm.o $(OBJ_DIR)/MaxCliqueAlgorithm.o \
                $(OBJ_DIR)/MSTAlgorithm.o $(OBJ_DIR)/HamiltonAlgorithm.o \
                $(OBJ_DIR)/SCCAlgorithm.o

CLIENT_OBJECTS = $(OBJ_DIR)/q7_client.o

# Create directories
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

all: server client

server: $(TARGET_SERVER)

client: $(TARGET_CLIENT)

$(TARGET_SERVER): $(SERVER_OBJECTS)
	$(CXX) $(SERVER_OBJECTS) -o $(TARGET_SERVER) -pthread
	@echo "✓ Q7 server: $(TARGET_SERVER)"

$(TARGET_CLIENT): $(CLIENT_OBJECTS)
	$(CXX) $(CLIENT_OBJECTS) -o $(TARGET_CLIENT)
	@echo "✓ Q7 client: $(TARGET_CLIENT)"

# Server objects
$(OBJ_DIR)/q7_server_main.o: server_main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c server_main.cpp -o $(OBJ_DIR)/q7_server_main.o

$(OBJ_DIR)/q7_client.o: client.cpp
	$(CXX) $(CXXFLAGS) -c client.cpp -o $(OBJ_DIR)/q7_client.o

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
	@echo "✓ Q7 cleaned"

test: all
	@echo "=== Q7 Test Instructions ==="
	@echo "1. Terminal 1: $(TARGET_SERVER)"
	@echo "2. Terminal 2: $(TARGET_CLIENT)"
	@echo "3. Try: euler 3 3 0-1 1-2 2-0"

run-server: server
	$(TARGET_SERVER)

run-client: client
	$(TARGET_CLIENT)

install: all
	@echo "✓ Q7 ready:"
	@echo "  Server: $(TARGET_SERVER) [port]"
	@echo "  Client: $(TARGET_CLIENT) [host] [port]"
	@echo ""
	@echo "Algorithms: euler, clique, mst, hamilton, scc"
	@echo "Uses Strategy + Factory patterns"

.PHONY: all server client clean test run-server run-client install

# Dependencies
$(OBJ_DIR)/q7_server_main.o: $(COMMON_DIR)/Graph.h $(ALGORITHMS_DIR)/AlgorithmFactory.h
$(OBJ_DIR)/Graph.o: $(COMMON_DIR)/Graph.h
$(OBJ_DIR)/AlgorithmFactory.o: $(ALGORITHMS_DIR)/AlgorithmStrategy.h
$(OBJ_DIR)/EulerAlgorithm.o: $(ALGORITHMS_DIR)/AlgorithmStrategy.h
$(OBJ_DIR)/MaxCliqueAlgorithm.o: $(ALGORITHMS_DIR)/AlgorithmStrategy.h
$(OBJ_DIR)/MSTAlgorithm.o: $(ALGORITHMS_DIR)/AlgorithmStrategy.h
$(OBJ_DIR)/HamiltonAlgorithm.o: $(ALGORITHMS_DIR)/AlgorithmStrategy.h
$(OBJ_DIR)/SCCAlgorithm.o: $(ALGORITHMS_DIR)/AlgorithmStrategy.h