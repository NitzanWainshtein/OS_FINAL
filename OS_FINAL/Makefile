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
