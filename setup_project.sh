#!/bin/bash

# Script to create OS_FINAL project structure WITHOUT duplications
# Usage: chmod +x setup_project.sh && ./setup_project.sh

echo "=== Setting up OS_FINAL Project Structure (No Duplications) ==="

# Create main directory (if not exists)
PROJECT_DIR="OS_FINAL"
if [ ! -d "$PROJECT_DIR" ]; then
    mkdir "$PROJECT_DIR"
fi

cd "$PROJECT_DIR"

# Clean existing content
echo "Cleaning existing content..."
rm -rf * .*ignore

# Create directory structure
echo "Creating directory structure..."

# Question directories - each contains ONLY what's unique to that question
mkdir -p q1 q2 q3 q4 q5 q6 q7 q8 q9

# Shared code directories
mkdir -p common         # Graph + GraphGenerator (shared by all)
mkdir -p algorithms     # All algorithm implementations (shared by q7,q8,q9)  
mkdir -p servers        # Server implementations (shared by q6,q7,q8,q9)
mkdir -p threading      # Threading utilities (shared by q8,q9)

# Build directories
mkdir -p bin obj reports

echo "Creating shared files (NO DUPLICATIONS)..."

# === COMMON FILES (used by multiple questions) ===
touch common/Graph.h
touch common/Graph.cpp
touch common/GraphGenerator.h
touch common/GraphGenerator.cpp

# === ALGORITHMS (used by q7,q8,q9) ===
touch algorithms/AlgorithmStrategy.h
touch algorithms/AlgorithmFactory.h
touch algorithms/AlgorithmFactory.cpp
touch algorithms/EulerAlgorithm.h
touch algorithms/EulerAlgorithm.cpp
touch algorithms/MaxCliqueAlgorithm.h
touch algorithms/MaxCliqueAlgorithm.cpp
touch algorithms/MSTAlgorithm.h
touch algorithms/MSTAlgorithm.cpp
touch algorithms/HamiltonAlgorithm.h
touch algorithms/HamiltonAlgorithm.cpp
touch algorithms/SCCAlgorithm.h
touch algorithms/SCCAlgorithm.cpp

# === SERVERS (used by q6,q7,q8,q9) ===
touch servers/BaseServer.h
touch servers/BaseServer.cpp
touch servers/EulerServer.h
touch servers/EulerServer.cpp
touch servers/AlgorithmsServer.h
touch servers/AlgorithmsServer.cpp

# === THREADING (used by q8,q9) ===
touch threading/ThreadPool.h
touch threading/ThreadPool.cpp
touch threading/ActiveObject.h
touch threading/ActiveObject.cpp

echo "Creating question-specific files..."

# === Q1: Basic Graph ONLY ===
touch q1/test_graph.cpp
touch q1/Makefile

# === Q2: Euler algorithm test ONLY ===
touch q2/test_euler.cpp
touch q2/Makefile

# === Q3: Random graph + getopt ONLY ===
touch q3/main.cpp
touch q3/Makefile

# === Q4: Coverage/Profiling scripts ONLY ===
touch q4/run_coverage.sh
touch q4/run_valgrind.sh
touch q4/run_profiling.sh
touch q4/Makefile

# === Q5: Problematic code for valgrind ONLY ===
touch q5/problematic_code.cpp
touch q5/Makefile
touch q5/valgrind_analysis.md

# === Q6: Server main + client ONLY ===
touch q6/server_main.cpp
touch q6/client.cpp
touch q6/Makefile

# === Q7: Factory/Strategy server main + client ONLY ===
touch q7/server_main.cpp
touch q7/client.cpp
touch q7/Makefile

# === Q8: Leader-Follower server ONLY ===
touch q8/LFServer.h
touch q8/LFServer.cpp
touch q8/server_main.cpp
touch q8/client.cpp
touch q8/Makefile

# === Q9: Pipeline server ONLY ===
touch q9/PipelineServer.h
touch q9/PipelineServer.cpp
touch q9/server_main.cpp
touch q9/client.cpp
touch q9/Makefile

# Create main project files
touch README.md
touch Makefile
touch .gitignore

# Make shell scripts executable
chmod +x q4/run_coverage.sh
chmod +x q4/run_valgrind.sh
chmod +x q4/run_profiling.sh

# Create README explaining the NO-DUPLICATION structure
cat > README.md << 'EOF'
# OS Final Project - Graph Algorithms (NO DUPLICATIONS)

## Smart Project Structure:
```
OS_FINAL/
├── common/           # Graph & GraphGenerator (shared by ALL)
├── algorithms/       # All algorithms (shared by q7,q8,q9)
├── servers/          # Server base classes (shared by q6,q7,q8,q9)
├── threading/        # Threading utilities (shared by q8,q9)
├── q1/               # ONLY: test_graph.cpp, Makefile
├── q2/               # ONLY: test_euler.cpp, Makefile
├── q3/               # ONLY: main.cpp (getopt), Makefile
├── q4/               # ONLY: coverage/profiling scripts
├── q5/               # ONLY: problematic_code.cpp
├── q6/               # ONLY: server_main.cpp, client.cpp
├── q7/               # ONLY: server_main.cpp, client.cpp
├── q8/               # ONLY: LFServer + main files
├── q9/               # ONLY: PipelineServer + main files
├── bin/              # All executables
└── obj/              # All object files
```

## Key Principle: ZERO DUPLICATIONS
- Graph.h/cpp appears ONCE in common/
- Each algorithm appears ONCE in algorithms/
- Servers inherit from BaseServer
- Each question builds on previous shared components

## Build Instructions:
```bash
# Build specific question:
cd qX && make

# Build all:
make all
```

Each Makefile knows how to include the shared components it needs.
EOF

# Create main Makefile that understands the shared structure
cat > Makefile << 'EOF'
# Main Makefile for OS Final Project (No Duplications)

.PHONY: all clean q1 q2 q3 q4 q5 q6 q7 q8 q9 clean-all

all: q1 q2 q3 q4 q5 q6 q7 q8 q9

# Each question builds using shared components
q1:
	@echo "Building Q1 (uses common/Graph)"
	$(MAKE) -C q1

q2:
	@echo "Building Q2 (uses common/Graph + Euler)"
	$(MAKE) -C q2

q3:
	@echo "Building Q3 (uses common/Graph + GraphGenerator)"
	$(MAKE) -C q3

q4:
	@echo "Running Q4 analysis scripts"
	$(MAKE) -C q4

q5:
	@echo "Building Q5 (problematic code for Valgrind)"
	$(MAKE) -C q5

q6:
	@echo "Building Q6 (uses common/ + servers/EulerServer)"
	$(MAKE) -C q6

q7:
	@echo "Building Q7 (uses common/ + algorithms/ + servers/)"
	$(MAKE) -C q7

q8:
	@echo "Building Q8 (uses all shared + threading/)"
	$(MAKE) -C q8

q9:
	@echo "Building Q9 (uses all shared + threading/ + pipeline)"
	$(MAKE) -C q9

clean:
	$(MAKE) -C q1 clean
	$(MAKE) -C q2 clean
	$(MAKE) -C q3 clean
	$(MAKE) -C q4 clean
	$(MAKE) -C q5 clean
	$(MAKE) -C q6 clean
	$(MAKE) -C q7 clean
	$(MAKE) -C q8 clean
	$(MAKE) -C q9 clean

clean-all: clean
	rm -rf bin/* obj/* reports/*

help:
	@echo "OS Final Project - No Duplications Structure"
	@echo ""
	@echo "Available targets:"
	@echo "  all       - Build all questions"
	@echo "  qX        - Build question X (1-9)"
	@echo "  clean     - Clean all build files"
	@echo "  clean-all - Clean everything including reports"
	@echo "  help      - Show this help"
	@echo ""
	@echo "Project uses shared components:"
	@echo "  common/     - Graph & GraphGenerator"
	@echo "  algorithms/ - All algorithm implementations"
	@echo "  servers/    - Server base classes"
	@echo "  threading/  - Threading utilities"
EOF

# Create .gitignore
cat > .gitignore << 'EOF'
# Build files
*.o
*.out
bin/*
obj/*
reports/*

# Coverage files
*.gcov
*.gcda
*.gcno

# Profiling files
gmon.out

# Valgrind files
*.vg

# IDE files
.vscode/
.idea/

# OS files
.DS_Store
Thumbs.db
EOF

echo ""
echo "=== NO-DUPLICATION Project Structure Created! ==="
echo ""
echo "Structure Summary:"
echo "📁 common/     - $(ls common/ | wc -l) shared files (Graph, GraphGenerator)"
echo "📁 algorithms/ - $(ls algorithms/ | wc -l) shared files (all algorithms)"  
echo "📁 servers/    - $(ls servers/ | wc -l) shared files (server base classes)"
echo "📁 threading/  - $(ls threading/ | wc -l) shared files (threading utilities)"
echo ""
echo "📁 q1/ - $(ls q1/ | wc -l) files (basic graph test)"
echo "📁 q2/ - $(ls q2/ | wc -l) files (euler test)"
echo "📁 q3/ - $(ls q3/ | wc -l) files (random + getopt)"
echo "📁 q4/ - $(ls q4/ | wc -l) files (analysis scripts)"
echo "📁 q5/ - $(ls q5/ | wc -l) files (problematic code)"
echo "📁 q6/ - $(ls q6/ | wc -l) files (euler server)"
echo "📁 q7/ - $(ls q7/ | wc -l) files (algorithms server)"
echo "📁 q8/ - $(ls q8/ | wc -l) files (leader-follower)"
echo "📁 q9/ - $(ls q9/ | wc -l) files (pipeline)"
echo ""
echo "✅ ZERO DUPLICATIONS!"
echo "✅ Each component appears exactly once"
echo "✅ Questions build incrementally using shared code"
echo ""
echo "Start with: cd q1 && [add content] && make"