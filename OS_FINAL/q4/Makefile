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