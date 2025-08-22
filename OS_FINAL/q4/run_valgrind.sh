echo "=== Q4: Valgrind Memory Analysis ==="
echo "Running Valgrind on Q3 random graph program..."

# Create reports directory
mkdir -p ../reports/valgrind

# Check if valgrind is available
if ! command -v valgrind >/dev/null 2>&1; then
    echo "❌ Valgrind not found. Please install valgrind."
    echo "Ubuntu/Debian: sudo apt-get install valgrind"
    echo "CentOS/RHEL: sudo yum install valgrind"
    exit 1
fi

# Compile with debug symbols (no optimization)
echo "Compiling with debug symbols..."
g++ -Wall -Wextra -std=c++17 -g -O0 \
    -I../common \
    main.cpp ../common/Graph.cpp ../common/GraphGenerator.cpp \
    -o ../bin/q4_valgrind_test

if [ $? -ne 0 ]; then
    echo "❌ Compilation failed"
    exit 1
fi

echo "✓ Compilation successful"

# Test cases for Valgrind analysis
echo ""
echo "Running Valgrind tests..."

echo "Test 1: Memcheck - Memory leak detection"
valgrind --tool=memcheck \
         --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=../reports/valgrind/memcheck.log \
         ../bin/q4_valgrind_test -v 10 -e 15 -s 42 -q

echo "✓ Memcheck completed - check ../reports/valgrind/memcheck.log"

echo ""
echo "Test 2: Callgrind - Performance profiling"
valgrind --tool=callgrind \
         --callgrind-out-file=../reports/valgrind/callgrind.out \
         ../bin/q4_valgrind_test -v 50 -e 100 -s 123 -q

echo "✓ Callgrind completed - check ../reports/valgrind/callgrind.out"

# Generate callgrind annotation if available
if command -v callgrind_annotate >/dev/null 2>&1; then
    callgrind_annotate ../reports/valgrind/callgrind.out > ../reports/valgrind/callgrind_annotation.txt
    echo "✓ Callgrind annotation: ../reports/valgrind/callgrind_annotation.txt"
fi

echo ""
echo "Test 3: Cachegrind - Cache performance analysis"
valgrind --tool=cachegrind \
         --cachegrind-out-file=../reports/valgrind/cachegrind.out \
         ../bin/q4_valgrind_test -v 100 -e 200 -s 456 -q

echo "✓ Cachegrind completed - check ../reports/valgrind/cachegrind.out"

# Analysis of results
echo ""
echo "=== Valgrind Analysis Summary ==="

echo "Memcheck Results:"
if [ -f "../reports/valgrind/memcheck.log" ]; then
    echo "Memory leaks:"
    grep -A5 "LEAK SUMMARY" ../reports/valgrind/memcheck.log
    echo ""
    echo "Error summary:"
    grep "ERROR SUMMARY" ../reports/valgrind/memcheck.log
else
    echo "❌ Memcheck log not found"
fi

echo ""
echo "Performance Results:"
if [ -f "../reports/valgrind/callgrind.out" ]; then
    echo "✓ Callgrind data available"
    if [ -f "../reports/valgrind/callgrind_annotation.txt" ]; then
        echo "Top functions by instruction count:"
        head -20 ../reports/valgrind/callgrind_annotation.txt
    fi
else
    echo "❌ Callgrind data not found"
fi

echo ""
echo "✓ Valgrind analysis completed!"
echo "Check ../reports/valgrind/ for detailed reports"
