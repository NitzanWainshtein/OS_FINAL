echo "=== Q4: Performance Profiling Analysis ==="
echo "Running gprof profiling on Q3 random graph program..."

# Create reports directory
mkdir -p ../reports/profiling

# Compile with profiling flags
echo "Compiling with profiling instrumentation..."
g++ -Wall -Wextra -std=c++17 -g -O0 -pg \
    -I../common \
    main.cpp ../common/Graph.cpp ../common/GraphGenerator.cpp \
    -o ../bin/q4_profiling_test

if [ $? -ne 0 ]; then
    echo "❌ Compilation failed"
    exit 1
fi

echo "✓ Compilation successful"

# Run test cases to generate profiling data
echo ""
echo "Running test cases for profiling..."

echo "Test 1: Small graphs (quick execution)"
../bin/q4_profiling_test -v 5 -e 8 -s 42 -q
../bin/q4_profiling_test -v 10 -e 15 -s 123 -q

echo "Test 2: Medium graphs"
../bin/q4_profiling_test -v 50 -e 100 -s 456 -q
../bin/q4_profiling_test -v 100 -e 200 -s 789 -q

echo "Test 3: Large graphs (for performance measurement)"
../bin/q4_profiling_test -v 500 -e 1000 -s 111 -q
../bin/q4_profiling_test -v 1000 -e 2000 -s 222 -q

echo "Test 4: Dense graphs (more computation)"
../bin/q4_profiling_test -v 100 -e 2000 -s 333 -q

echo "Test 5: Directed graphs"
../bin/q4_profiling_test -v 200 -e 800 -s 444 -d -q

# Generate profiling report
echo ""
echo "Generating profiling reports..."

if [ -f "gmon.out" ]; then
    # Generate text report
    gprof ../bin/q4_profiling_test gmon.out > ../reports/profiling/gprof_report.txt
    
    # Generate call graph
    gprof -A ../bin/q4_profiling_test gmon.out > ../reports/profiling/gprof_annotated.txt
    
    # Generate flat profile
    gprof -p ../bin/q4_profiling_test gmon.out > ../reports/profiling/gprof_flat.txt
    
    # Generate call graph only
    gprof -q ../bin/q4_profiling_test gmon.out > ../reports/profiling/gprof_callgraph.txt
    
    # Move gmon.out to reports
    mv gmon.out ../reports/profiling/
    
    echo "✓ Profiling reports generated successfully"
else
    echo "❌ gmon.out not found - profiling data not generated"
    exit 1
fi

# Analysis summary
echo ""
echo "=== Profiling Analysis Summary ==="

echo "Function Performance (top 10):"
head -20 ../reports/profiling/gprof_flat.txt | tail -10

echo ""
echo "Call Graph Summary:"
head -30 ../reports/profiling/gprof_callgraph.txt | tail -15

echo ""
echo "Files generated:"
ls -la ../reports/profiling/

echo ""
echo "✓ Performance profiling completed!"
echo "Check ../reports/profiling/ for detailed reports"
echo ""
echo "Key files:"
echo "- gprof_report.txt: Complete profiling report"
echo "- gprof_flat.txt: Function performance ranking"
echo "- gprof_callgraph.txt: Function call relationships"
echo "- gprof_annotated.txt: Annotated source code"

# ===== q4/Makefile =====
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -O0
INCLUDES = -I../common

# Directories
COMMON_DIR = ../common
OBJ_DIR = ../obj
BIN_DIR = ../bin
REPORTS_DIR = ../reports

# Targets
TARGET_COVERAGE = $(BIN_DIR)/q4_coverage_test
TARGET_VALGRIND = $(BIN_DIR)/q4_valgrind_test
TARGET_PROFILING = $(BIN_DIR)/q4_profiling_test

# Source files (reuse Q3's main.cpp)
SOURCES = main.cpp $(COMMON_DIR)/Graph.cpp $(COMMON_DIR)/GraphGenerator.cpp

# Create directories
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR) $(REPORTS_DIR)/coverage $(REPORTS_DIR)/valgrind $(REPORTS_DIR)/profiling)

all: coverage valgrind profiling

# Coverage target
coverage: $(TARGET_COVERAGE)
	@echo "✓ Coverage build ready"

$(TARGET_COVERAGE): $(SOURCES)
	$(CXX) $(CXXFLAGS) --coverage -fprofile-arcs -ftest-coverage $(INCLUDES) $(SOURCES) -o $(TARGET_COVERAGE)

# Valgrind target  
valgrind: $(TARGET_VALGRIND)
	@echo "✓ Valgrind build ready"

$(TARGET_VALGRIND): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES) -o $(TARGET_VALGRIND)

# Profiling target
profiling: $(TARGET_PROFILING)
	@echo "✓ Profiling build ready"

$(TARGET_PROFILING): $(SOURCES)
	$(CXX) $(CXXFLAGS) -pg $(INCLUDES) $(SOURCES) -o $(TARGET_PROFILING)

# Copy main.cpp from q3 if it doesn't exist
main.cpp:
	cp ../q3/main.cpp .
	@echo "✓ Copied main.cpp from Q3"

# Run all analyses
test: all run-coverage run-valgrind run-profiling

run-coverage: coverage
	@echo "Running coverage analysis..."
	chmod +x run_coverage.sh
	./run_coverage.sh

run-valgrind: valgrind  
	@echo "Running Valgrind analysis..."
	chmod +x run_valgrind.sh
	./run_valgrind.sh

run-profiling: profiling
	@echo "Running profiling analysis..."
	chmod +x run_profiling.sh
	./run_profiling.sh

# Individual quick tests
test-coverage: coverage
	chmod +x run_coverage.sh
	./run_coverage.sh

test-valgrind: valgrind
	chmod +x run_valgrind.sh  
	./run_valgrind.sh

test-profiling: profiling
	chmod +x run_profiling.sh
	./run_profiling.sh

clean:
	rm -f $(TARGET_COVERAGE) $(TARGET_VALGRIND) $(TARGET_PROFILING)
	rm -f *.gcov *.gcda *.gcno gmon.out
	rm -f main.cpp  # Remove copied file
	rm -rf $(REPORTS_DIR)/coverage/* $(REPORTS_DIR)/valgrind/* $(REPORTS_DIR)/profiling/*
	@echo "✓ Q4 cleaned"

clean-reports:
	rm -rf $(REPORTS_DIR)/coverage/* $(REPORTS_DIR)/valgrind/* $(REPORTS_DIR)/profiling/*
	@echo "✓ All reports cleaned"

install: all
	@echo "✓ Q4 analysis tools ready"
	@echo ""
	@echo "Available commands:"
	@echo "  make test-coverage  - Run code coverage analysis"
	@echo "  make test-valgrind  - Run Valgrind memory analysis"  
	@echo "  make test-profiling - Run performance profiling"
	@echo "  make test          - Run all analyses"
	@echo ""
	@echo "Reports will be saved in ../reports/"

.PHONY: all coverage valgrind profiling test run-coverage run-valgrind run-profiling
.PHONY: test-coverage test-valgrind test-profiling clean clean-reports install