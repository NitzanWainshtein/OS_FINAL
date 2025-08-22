echo "=== Q4: Code Coverage Analysis ==="
echo "Generating coverage report for Q3 random graph program..."

# Create reports directory
mkdir -p ../reports/coverage

# Compile with coverage flags
echo "Compiling with coverage instrumentation..."
g++ -Wall -Wextra -std=c++17 -g -O0 --coverage -fprofile-arcs -ftest-coverage \
    -I../common \
    main.cpp ../common/Graph.cpp ../common/GraphGenerator.cpp \
    -o ../bin/q4_coverage_test

if [ $? -ne 0 ]; then
    echo "❌ Compilation failed"
    exit 1
fi

echo "✓ Compilation successful"

# Run various test cases to maximize coverage
echo ""
echo "Running test cases to generate coverage data..."

echo "Test 1: Triangle (has Euler circuit)"
../bin/q4_coverage_test -v 3 -e 3 -s 42 -q

echo "Test 2: Path (no Euler circuit)"
../bin/q4_coverage_test -v 4 -e 3 -s 123 -q

echo "Test 3: Directed graph"
../bin/q4_coverage_test -v 5 -e 8 -s 456 -d -q

echo "Test 4: Empty graph"
../bin/q4_coverage_test -v 5 -e 0 -s 789 -q

echo "Test 5: Single vertex"
../bin/q4_coverage_test -v 1 -e 0 -s 111 -q

echo "Test 6: Complete small graph"
../bin/q4_coverage_test -v 4 -e 6 -s 222 -q

echo "Test 7: Large graph"
../bin/q4_coverage_test -v 20 -e 50 -s 333 -q

echo "Test 8: Error cases"
../bin/q4_coverage_test -v -1 -e 5 -s 42 -q 2>/dev/null || true
../bin/q4_coverage_test -v 5 -e -1 -s 42 -q 2>/dev/null || true
../bin/q4_coverage_test -v 3 -e 10 -s 42 -q 2>/dev/null || true

echo "Test 9: Help and verbose modes"
../bin/q4_coverage_test --help >/dev/null
../bin/q4_coverage_test -v 3 -e 3 -s 42 -V >/dev/null

echo ""
echo "Generating coverage reports..."

# Generate coverage report with gcov
gcov -b -c main.cpp Graph.cpp GraphGenerator.cpp 2>/dev/null

# Move coverage files to reports directory
mv *.gcov ../reports/coverage/ 2>/dev/null || true
mv *.gcda ../reports/coverage/ 2>/dev/null || true
mv *.gcno ../reports/coverage/ 2>/dev/null || true

# Generate HTML report if lcov is available
if command -v lcov >/dev/null 2>&1; then
    echo "Generating HTML coverage report with lcov..."
    lcov --capture --directory . --output-file ../reports/coverage/coverage.info
    genhtml ../reports/coverage/coverage.info --output-directory ../reports/coverage/html
    echo "✓ HTML coverage report: ../reports/coverage/html/index.html"
else
    echo "⚠ lcov not available, skipping HTML report"
fi

# Summary
echo ""
echo "=== Coverage Analysis Summary ==="
if [ -f "../reports/coverage/main.cpp.gcov" ]; then
    echo "✓ Coverage files generated successfully"
    echo "Files:"
    ls -la ../reports/coverage/
    
    # Show basic coverage stats
    echo ""
    echo "Coverage Summary:"
    grep -E "####|[0-9]+:" ../reports/coverage/main.cpp.gcov | head -20
else
    echo "❌ Coverage generation failed"
    exit 1
fi

echo ""
echo "✓ Coverage analysis completed!"
echo "Check ../reports/coverage/ for detailed reports"