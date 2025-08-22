CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -O0 -pthread
INCLUDES = -I../common

# Directories
COMMON_DIR = ../common
OBJ_DIR = ../obj
BIN_DIR = ../bin

# Targets
TARGET_SERVER = $(BIN_DIR)/q6_server
TARGET_CLIENT = $(BIN_DIR)/q6_client

# Sources
SERVER_SOURCES = server_main.cpp $(COMMON_DIR)/Graph.cpp
CLIENT_SOURCES = client.cpp

# Objects
SERVER_OBJECTS = $(OBJ_DIR)/q6_server_main.o $(OBJ_DIR)/Graph.o
CLIENT_OBJECTS = $(OBJ_DIR)/q6_client.o

# Create directories
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

all: server client

server: $(TARGET_SERVER)

client: $(TARGET_CLIENT)

$(TARGET_SERVER): $(SERVER_OBJECTS)
	$(CXX) $(SERVER_OBJECTS) -o $(TARGET_SERVER) -pthread
	@echo "✓ Q6 server: $(TARGET_SERVER)"

$(TARGET_CLIENT): $(CLIENT_OBJECTS)
	$(CXX) $(CLIENT_OBJECTS) -o $(TARGET_CLIENT)
	@echo "✓ Q6 client: $(TARGET_CLIENT)"

$(OBJ_DIR)/q6_server_main.o: server_main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c server_main.cpp -o $(OBJ_DIR)/q6_server_main.o

$(OBJ_DIR)/q6_client.o: client.cpp
	$(CXX) $(CXXFLAGS) -c client.cpp -o $(OBJ_DIR)/q6_client.o

$(OBJ_DIR)/Graph.o: $(COMMON_DIR)/Graph.cpp $(COMMON_DIR)/Graph.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(COMMON_DIR)/Graph.cpp -o $(OBJ_DIR)/Graph.o

clean:
	rm -f $(SERVER_OBJECTS) $(CLIENT_OBJECTS) $(TARGET_SERVER) $(TARGET_CLIENT)
	@echo "✓ Q6 cleaned"

test: all
	@echo "=== Q6 Test Instructions ==="
	@echo "1. Terminal 1: $(TARGET_SERVER)"
	@echo "2. Terminal 2: $(TARGET_CLIENT)"
	@echo "3. Or use telnet: telnet localhost 8080"

run-server: server
	$(TARGET_SERVER)

run-client: client
	$(TARGET_CLIENT)

install: all
	@echo "✓ Q6 ready:"
	@echo "  Server: $(TARGET_SERVER) [port]"
	@echo "  Client: $(TARGET_CLIENT) [host] [port]"

.PHONY: all server client clean test run-server run-client install

# Dependencies
$(OBJ_DIR)/q6_server_main.o: $(COMMON_DIR)/Graph.h
$(OBJ_DIR)/Graph.o: $(COMMON_DIR)/Graph.h