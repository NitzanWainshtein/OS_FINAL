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
