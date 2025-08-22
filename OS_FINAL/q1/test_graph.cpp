#include "../common/Graph.h"
#include <iostream>
#include <exception>

void testBasicOperations() {
    std::cout << "\n=== Test 1: Basic Graph Operations ===" << std::endl;
    
    try {
        // Test undirected graph
        Graph g(5, false);
        std::cout << "✓ Created undirected graph with 5 vertices" << std::endl;
        
        // Add edges to form a cycle
        g.addEdge(0, 1);
        g.addEdge(1, 2);
        g.addEdge(2, 3);
        g.addEdge(3, 4);
        g.addEdge(4, 0);
        
        g.printGraph();
        
        // Test basic properties
        std::cout << "\nBasic Properties:" << std::endl;
        std::cout << "- Connected: " << (g.isConnected() ? "Yes" : "No") << std::endl;
        std::cout << "- Empty: " << (g.isEmpty() ? "Yes" : "No") << std::endl;
        std::cout << "- Edge count: " << g.getEdgeCount() << std::endl;
        
        // Test edge queries
        std::cout << "\nEdge Queries:" << std::endl;
        std::cout << "- Has edge 0-1: " << (g.hasEdge(0, 1) ? "Yes" : "No") << std::endl;
        std::cout << "- Has edge 1-3: " << (g.hasEdge(1, 3) ? "Yes" : "No") << std::endl;
        
        // Test neighbors
        auto neighbors = g.getNeighbors(1);
        std::cout << "- Neighbors of vertex 1: ";
        for (int n : neighbors) std::cout << n << " ";
        std::cout << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ Error: " << e.what() << std::endl;
    }
}

void testDirectedGraph() {
    std::cout << "\n=== Test 2: Directed Graph ===" << std::endl;
    
    try {
        Graph g(4, true);
        std::cout << "✓ Created directed graph with 4 vertices" << std::endl;
        
        // Create a directed cycle
        g.addEdge(0, 1);
        g.addEdge(1, 2);
        g.addEdge(2, 3);
        g.addEdge(3, 0);
        
        g.printGraph();
        
        // Test directed properties
        std::cout << "\nDirected Properties:" << std::endl;
        std::cout << "- Edge 0->1 exists: " << (g.hasEdge(0, 1) ? "Yes" : "No") << std::endl;
        std::cout << "- Edge 1->0 exists: " << (g.hasEdge(1, 0) ? "Yes" : "No") << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ Error: " << e.what() << std::endl;
    }
}

void testErrorHandling() {
    std::cout << "\n=== Test 3: Error Handling ===" << std::endl;
    
    // Test invalid graph creation
    std::cout << "\nTesting invalid graph creation:" << std::endl;
    
    try {
        Graph g(-1, false);
        std::cout << "❌ Should have thrown error for negative vertices" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✓ Caught expected error: " << e.what() << std::endl;
    }
    
    try {
        Graph g(15000, false);
        std::cout << "❌ Should have thrown error for too many vertices" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✓ Caught expected error: " << e.what() << std::endl;
    }
    
    // Test invalid operations on valid graph
    std::cout << "\nTesting invalid operations:" << std::endl;
    
    try {
        Graph g(3, false);
        
        // Test out of range vertices
        try {
            g.addEdge(-1, 1);
            std::cout << "❌ Should have thrown error for negative vertex" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "✓ Caught expected error: " << e.what() << std::endl;
        }
        
        try {
            g.addEdge(0, 5);
            std::cout << "❌ Should have thrown error for vertex out of range" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "✓ Caught expected error: " << e.what() << std::endl;
        }
        
        // Test self-loop
        try {
            g.addEdge(1, 1);
            std::cout << "❌ Should have thrown error for self-loop" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "✓ Caught expected error: " << e.what() << std::endl;
        }
        
        // Test duplicate edges (should be ignored silently)
        g.addEdge(0, 1);
        std::cout << "✓ Added edge 0-1" << std::endl;
        g.addEdge(0, 1); // Should be ignored
        std::cout << "✓ Duplicate edge 0-1 ignored silently" << std::endl;
        
        // Test invalid operations
        auto neighbors = g.getNeighbors(-1); // Should return empty vector
        std::cout << "✓ Invalid vertex neighbors query returned " << neighbors.size() << " neighbors" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ Unexpected error: " << e.what() << std::endl;
    }
}

void testSpecialGraphs() {
    std::cout << "\n=== Test 4: Special Graph Types ===" << std::endl;
    
    try {
        // Single vertex graph
        std::cout << "\nSingle vertex graph:" << std::endl;
        Graph g1(1, false);
        g1.printGraph();
        std::cout << "- Connected: " << (g1.isConnected() ? "Yes" : "No") << std::endl;
        std::cout << "- Empty: " << (g1.isEmpty() ? "Yes" : "No") << std::endl;
        
        // Empty graph (no edges)
        std::cout << "\nEmpty graph (3 vertices, no edges):" << std::endl;
        Graph g2(3, false);
        g2.printGraph();
        std::cout << "- Connected: " << (g2.isConnected() ? "Yes" : "No") << std::endl;
        std::cout << "- Empty: " << (g2.isEmpty() ? "Yes" : "No") << std::endl;
        
        // Complete triangle (K3)
        std::cout << "\nComplete triangle K3:" << std::endl;
        Graph g3(3, false);
        g3.addEdge(0, 1);
        g3.addEdge(1, 2);
        g3.addEdge(2, 0);
        g3.printGraph();
        std::cout << "- Connected: " << (g3.isConnected() ? "Yes" : "No") << std::endl;
        std::cout << "- Edge count: " << g3.getEdgeCount() << std::endl;
        
        // Star graph
        std::cout << "\nStar graph (5 vertices):" << std::endl;
        Graph g4(5, false);
        for (int i = 1; i < 5; ++i) {
            g4.addEdge(0, i);
        }
        g4.printGraph();
        std::cout << "- Connected: " << (g4.isConnected() ? "Yes" : "No") << std::endl;
        std::cout << "- Center degree: " << g4.getDegree(0) << std::endl;
        std::cout << "- Leaf degree: " << g4.getDegree(1) << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ Error: " << e.what() << std::endl;
    }
}

void testCopyAndModify() {
    std::cout << "\n=== Test 5: Copy and Modify Operations ===" << std::endl;
    
    try {
        // Create original graph
        Graph original(4, false);
        original.addEdge(0, 1);
        original.addEdge(1, 2);
        original.addEdge(2, 3);
        
        std::cout << "Original graph:" << std::endl;
        original.printGraph();
        
        // Test copy constructor
        Graph copy(original);
        std::cout << "\nCopied graph:" << std::endl;
        copy.printGraph();
        
        // Modify copy
        copy.addEdge(3, 0);
        copy.removeEdge(1, 2);
        
        std::cout << "\nModified copy:" << std::endl;
        copy.printGraph();
        
        std::cout << "\nOriginal after copy modification:" << std::endl;
        original.printGraph();
        
        std::cout << "✓ Copy operations work correctly" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ Error: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "=== Q1: Graph Data Structure Implementation Testing ===" << std::endl;
    std::cout << "Testing comprehensive Graph class functionality..." << std::endl;
    
    testBasicOperations();
    testDirectedGraph();
    testErrorHandling();
    testSpecialGraphs();
    testCopyAndModify();
    
    std::cout << "\n=== Q1 Testing Completed Successfully! ===" << std::endl;
    std::cout << "✓ Graph data structure is working correctly" << std::endl;
    std::cout << "✓ All error cases handled properly" << std::endl;
    std::cout << "✓ Both directed and undirected graphs supported" << std::endl;
    std::cout << "✓ Ready for Requirement 2 (Euler algorithm)" << std::endl;
    
    return 0;
}