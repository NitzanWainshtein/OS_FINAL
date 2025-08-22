CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -O0
INCLUDES = -I../common

# Directories
COMMON_DIR = ../common
OBJ_DIR = ../obj
BIN_DIR = ../bin

# Target
TARGET = $(BIN_DIR)/q1_test_graph

# Source files
SOURCES = test_graph.cpp $(COMMON_DIR)/Graph.cpp
OBJECTS = $(OBJ_DIR)/q1_test_graph.o $(OBJ_DIR)/Graph.o

# Create directories if they don't exist
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo "✓ Q1 executable created: $(TARGET)"

$(OBJ_DIR)/q1_test_graph.o: test_graph.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c test_graph.cpp -o $(OBJ_DIR)/q1_test_graph.o

$(OBJ_DIR)/Graph.o: $(COMMON_DIR)/Graph.cpp $(COMMON_DIR)/Graph.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(COMMON_DIR)/Graph.cpp -o $(OBJ_DIR)/Graph.o

clean:
	rm -f $(OBJ_DIR)/q1_test_graph.o $(OBJ_DIR)/Graph.o $(TARGET)
	@echo "✓ Q1 cleaned"

test: $(TARGET)
	@echo "Running Q1 tests..."
	$(TARGET)

install: $(TARGET)
	@echo "✓ Q1 ready to run: $(TARGET)"

.PHONY: all clean test install

# Dependencies
$(OBJ_DIR)/q1_test_graph.o: $(COMMON_DIR)/Graph.h
$(OBJ_DIR)/Graph.o: $(COMMON_DIR)/Graph.h