CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -O0
INCLUDES = -I../common

# Directories
COMMON_DIR = ../common
OBJ_DIR = ../obj
BIN_DIR = ../bin

# Target
TARGET = $(BIN_DIR)/q3_random_graph

# Source files
SOURCES = main.cpp $(COMMON_DIR)/Graph.cpp $(COMMON_DIR)/GraphGenerator.cpp
OBJECTS = $(OBJ_DIR)/q3_main.o $(OBJ_DIR)/Graph.o $(OBJ_DIR)/GraphGenerator.o

# Create directories if they don't exist
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo "✓ Q3 executable created: $(TARGET)"

$(OBJ_DIR)/q3_main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c main.cpp -o $(OBJ_DIR)/q3_main.o

$(OBJ_DIR)/Graph.o: $(COMMON_DIR)/Graph.cpp $(COMMON_DIR)/Graph.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(COMMON_DIR)/Graph.cpp -o $(OBJ_DIR)/Graph.o

$(OBJ_DIR)/GraphGenerator.o: $(COMMON_DIR)/GraphGenerator.cpp $(COMMON_DIR)/GraphGenerator.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(COMMON_DIR)/GraphGenerator.cpp -o $(OBJ_DIR)/GraphGenerator.o

clean:
	rm -f $(OBJ_DIR)/q3_main.o $(TARGET)
	@echo "✓ Q3 cleaned"

test: $(TARGET)
	@echo "Running Q3 tests..."
	@echo "Test 1: Small triangle (should have Euler circuit)"
	$(TARGET) -v 3 -e 3 -s 42
	@echo ""
	@echo "Test 2: Path graph (should NOT have Euler circuit)"
	$(TARGET) -v 4 -e 3 -s 123 -q
	@echo ""
	@echo "Test 3: Directed graph"
	$(TARGET) -v 5 -e 8 -s 456 -d
	@echo ""
	@echo "Test 4: Larger random graph"
	$(TARGET) -v 10 -e 15 -s 789 -V

# Quick tests for different scenarios
test-quick: $(TARGET)
	@echo "Quick tests..."
	$(TARGET) -v 4 -e 4 -s 1 -q
	$(TARGET) -v 6 -e 9 -s 2 -q

# Test error handling
test-errors: $(TARGET)
	@echo "Testing error handling..."
	-$(TARGET) -v -1 -e 5 -s 42 2>/dev/null || echo "✓ Negative vertices rejected"
	-$(TARGET) -v 5 -e -1 -s 42 2>/dev/null || echo "✓ Negative edges rejected"
	-$(TARGET) -v 3 -e 10 -s 42 2>/dev/null || echo "✓ Too many edges rejected"
	-$(TARGET) -v 5 2>/dev/null || echo "✓ Missing arguments rejected"

install: $(TARGET)
	@echo "✓ Q3 ready to run: $(TARGET)"
	@echo ""
	@echo "Example usage:"
	@echo "  $(TARGET) -v 5 -e 6 -s 42"
	@echo "  $(TARGET) --vertices 10 --edges 15 --seed 123 --directed"
	@echo "  $(TARGET) -v 4 -e 4 -s 1 --quiet"

.PHONY: all clean test test-quick test-errors install

# Dependencies
$(OBJ_DIR)/q3_main.o: $(COMMON_DIR)/Graph.h $(COMMON_DIR)/GraphGenerator.h
$(OBJ_DIR)/Graph.o: $(COMMON_DIR)/Graph.h
$(OBJ_DIR)/GraphGenerator.o: $(COMMON_DIR)/GraphGenerator.h $(COMMON_DIR)/Graph.h