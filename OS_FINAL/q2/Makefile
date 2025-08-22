CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -O0
INCLUDES = -I../common

# Directories
COMMON_DIR = ../common
OBJ_DIR = ../obj
BIN_DIR = ../bin

# Target
TARGET = $(BIN_DIR)/q2_test_euler

# Source files
SOURCES = test_euler.cpp $(COMMON_DIR)/Graph.cpp
OBJECTS = $(OBJ_DIR)/q2_test_euler.o $(OBJ_DIR)/Graph.o

# Create directories if they don't exist
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo "✓ Q2 executable created: $(TARGET)"

$(OBJ_DIR)/q2_test_euler.o: test_euler.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c test_euler.cpp -o $(OBJ_DIR)/q2_test_euler.o

$(OBJ_DIR)/Graph.o: $(COMMON_DIR)/Graph.cpp $(COMMON_DIR)/Graph.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(COMMON_DIR)/Graph.cpp -o $(OBJ_DIR)/Graph.o

clean:
	rm -f $(OBJ_DIR)/q2_test_euler.o $(TARGET)
	@echo "✓ Q2 cleaned"

test: $(TARGET)
	@echo "Running Q2 tests..."
	$(TARGET)

install: $(TARGET)
	@echo "✓ Q2 ready to run: $(TARGET)"

.PHONY: all clean test install

# Dependencies
$(OBJ_DIR)/q2_test_euler.o: $(COMMON_DIR)/Graph.h
$(OBJ_DIR)/Graph.o: $(COMMON_DIR)/Graph.h