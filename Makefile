# Makefile for OS Project - Essential Features Only
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -pthread

# Analysis flags
COVERAGE_FLAGS = --coverage -fprofile-arcs -ftest-coverage
PROFILE_FLAGS = -pg

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin
REPORTDIR = reports

# Source files
GRAPH_SOURCES = $(SRCDIR)/graph/Graph.cpp $(SRCDIR)/graph/GraphGenerator.cpp
ALGORITHM_SOURCES = $(SRCDIR)/algorithms/EulerAlgorithm.cpp \
                   $(SRCDIR)/algorithms/MSTAlgorithm.cpp \
                   $(SRCDIR)/algorithms/MaxCliqueAlgorithm.cpp \
                   $(SRCDIR)/algorithms/HamiltonAlgorithm.cpp \
                   $(SRCDIR)/algorithms/SCCAlgorithm.cpp \
                   $(SRCDIR)/algorithms/AlgorithmFactory.cpp

# Object files
GRAPH_OBJECTS = $(GRAPH_SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
ALGORITHM_OBJECTS = $(ALGORITHM_SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Main targets
.PHONY: all clean basic euler algorithms tcp-server simple-client coverage profile valgrind

all: basic euler algorithms tcp-server simple-client

# Requirements 1-4: Basic graph with Euler
basic: $(BINDIR)/basic_graph

$(BINDIR)/basic_graph: $(GRAPH_OBJECTS) $(OBJDIR)/main.o | $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Requirement 6: Euler server
euler: $(BINDIR)/euler_server

$(BINDIR)/euler_server: $(GRAPH_OBJECTS) $(OBJDIR)/server/EulerServer.o $(OBJDIR)/server/main_euler.o | $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Requirement 7: Algorithms server
algorithms: $(BINDIR)/algorithms_server

$(BINDIR)/algorithms_server: $(GRAPH_OBJECTS) $(ALGORITHM_OBJECTS) $(OBJDIR)/server/AlgorithmsServer.o $(OBJDIR)/server/main_algorithms.o | $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# TCP Server
tcp-server: $(BINDIR)/tcp_server

$(BINDIR)/tcp_server: $(GRAPH_OBJECTS) $(OBJDIR)/server/EulerServer.o $(OBJDIR)/server/tcp_main.o | $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# TCP Client
simple-client: $(BINDIR)/simple_client

$(BINDIR)/simple_client: $(OBJDIR)/client/simple_client.o | $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Object file compilation
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories
$(OBJDIR):
	mkdir -p $(OBJDIR)/graph $(OBJDIR)/algorithms $(OBJDIR)/server $(OBJDIR)/client

$(BINDIR):
	mkdir -p $(BINDIR)

$(REPORTDIR):
	mkdir -p $(REPORTDIR)

# ========== ANALYSIS TOOLS (Requirement 4) ==========

coverage: | $(REPORTDIR)
	@echo "=== Building with coverage flags ==="
	rm -rf $(OBJDIR) $(BINDIR) *.gcov *.gcda *.gcno
	mkdir -p $(OBJDIR)/graph $(OBJDIR)/algorithms $(OBJDIR)/server $(BINDIR)
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) -c $(SRCDIR)/graph/Graph.cpp -o $(OBJDIR)/graph/Graph.o
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) -c $(SRCDIR)/graph/GraphGenerator.cpp -o $(OBJDIR)/graph/GraphGenerator.o
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) -c $(SRCDIR)/main.cpp -o $(OBJDIR)/main.o
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) $(OBJDIR)/graph/*.o $(OBJDIR)/main.o -o $(BINDIR)/basic_graph
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) -c $(SRCDIR)/algorithms/EulerAlgorithm.cpp -o $(OBJDIR)/algorithms/EulerAlgorithm.o
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) -c $(SRCDIR)/algorithms/MSTAlgorithm.cpp -o $(OBJDIR)/algorithms/MSTAlgorithm.o
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) -c $(SRCDIR)/algorithms/MaxCliqueAlgorithm.cpp -o $(OBJDIR)/algorithms/MaxCliqueAlgorithm.o
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) -c $(SRCDIR)/algorithms/HamiltonAlgorithm.cpp -o $(OBJDIR)/algorithms/HamiltonAlgorithm.o
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) -c $(SRCDIR)/algorithms/SCCAlgorithm.cpp -o $(OBJDIR)/algorithms/SCCAlgorithm.o
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) -c $(SRCDIR)/algorithms/AlgorithmFactory.cpp -o $(OBJDIR)/algorithms/AlgorithmFactory.o
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) -c $(SRCDIR)/server/AlgorithmsServer.cpp -o $(OBJDIR)/server/AlgorithmsServer.o
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) -c $(SRCDIR)/server/main_algorithms.cpp -o $(OBJDIR)/server/main_algorithms.o
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) $(OBJDIR)/graph/*.o $(OBJDIR)/algorithms/*.o $(OBJDIR)/server/AlgorithmsServer.o $(OBJDIR)/server/main_algorithms.o -o $(BINDIR)/algorithms_server
	@echo "Running tests..."
	./$(BINDIR)/basic_graph -v 4 -e 6 -s 123
	./$(BINDIR)/basic_graph -v 3 -e 3 -s 100
	timeout 2s ./$(BINDIR)/algorithms_server || true
	@echo "Generating coverage report..."
	gcov $(SRCDIR)/*/*.cpp -o $(OBJDIR) 2>/dev/null || true
	lcov --capture --directory . --output-file $(REPORTDIR)/coverage.info --ignore-errors all 2>/dev/null || true
	genhtml $(REPORTDIR)/coverage.info --output-directory $(REPORTDIR)/coverage_html --ignore-errors all 2>/dev/null || true
	@echo "Coverage report: $(REPORTDIR)/coverage_html/index.html"

# Performance Profiling
profile: | $(REPORTDIR)
	@echo "=== Building with profiling flags ==="
	rm -rf $(OBJDIR) $(BINDIR) gmon.out
	mkdir -p $(OBJDIR)/graph $(BINDIR)
	$(CXX) $(CXXFLAGS) $(PROFILE_FLAGS) -c $(SRCDIR)/graph/Graph.cpp -o $(OBJDIR)/graph/Graph.o
	$(CXX) $(CXXFLAGS) $(PROFILE_FLAGS) -c $(SRCDIR)/graph/GraphGenerator.cpp -o $(OBJDIR)/graph/GraphGenerator.o
	$(CXX) $(CXXFLAGS) $(PROFILE_FLAGS) -c $(SRCDIR)/main.cpp -o $(OBJDIR)/main.o
	$(CXX) $(CXXFLAGS) $(PROFILE_FLAGS) $(OBJDIR)/graph/*.o $(OBJDIR)/main.o -o $(BINDIR)/basic_graph
	@echo "Running profiled execution..."
	./$(BINDIR)/basic_graph -v 10 -e 20 -s 777
	./$(BINDIR)/basic_graph -v 8 -e 15 -s 888
	@if [ -f gmon.out ]; then \
		gprof $(BINDIR)/basic_graph gmon.out > $(REPORTDIR)/gprof_report.txt; \
		echo "Profile report: $(REPORTDIR)/gprof_report.txt"; \
	else \
		echo "No profiling data - creating placeholder"; \
		echo "No profiling data collected" > $(REPORTDIR)/gprof_report.txt; \
	fi

# Valgrind Analysis
valgrind: basic | $(REPORTDIR)
	@echo "=== Running Valgrind Analysis ==="
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --log-file=$(REPORTDIR)/valgrind_memcheck.log \
		./$(BINDIR)/basic_graph -v 5 -e 7 -s 456
	valgrind --tool=callgrind --callgrind-out-file=$(REPORTDIR)/callgrind.out \
		./$(BINDIR)/basic_graph -v 4 -e 6 -s 789
	callgrind_annotate $(REPORTDIR)/callgrind.out > $(REPORTDIR)/callgrind_report.txt 2>/dev/null || true
	@echo "Valgrind reports: $(REPORTDIR)/valgrind_memcheck.log, $(REPORTDIR)/callgrind_report.txt"

# Run all analysis
analyze: coverage profile valgrind
	@echo "=== Analysis Complete ==="
	@ls -la $(REPORTDIR)/

# ========== TESTS ==========

test-basic: basic
	./$(BINDIR)/basic_graph -v 5 -e 7 -s 123

test-tcp: tcp-server simple-client
	@echo "TCP components built. Run:"
	@echo "Terminal 1: ./$(BINDIR)/tcp_server"
	@echo "Terminal 2: ./$(BINDIR)/simple_client"

# Clean
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(REPORTDIR) *.gcov *.gcda *.gcno gmon.out

# Help
help:
	@echo "Essential targets:"
	@echo "  all         - Build everything"
	@echo "  basic       - Build basic graph (Requirements 1-4)"
	@echo "  euler       - Build Euler server (Requirement 6)"
	@echo "  algorithms  - Build algorithms server (Requirement 7)"
	@echo "  tcp-server  - Build TCP server"
	@echo "  simple-client - Build TCP client"
	@echo "  coverage    - Generate code coverage (Requirement 4)"
	@echo "  profile     - Generate gprof profile (Requirement 4)"
	@echo "  valgrind    - Run Valgrind analysis (Requirement 4)"
	@echo "  analyze     - Run all analysis tools"
	@echo "  clean       - Remove all build files"