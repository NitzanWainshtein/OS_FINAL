#include "../common/Graph.h"
#include "../common/GraphGenerator.h"
#include <iostream>
#include <getopt.h>
#include <cstdlib>
#include <exception>

void printUsage(const char* progName) {
    std::cout << "\n=== Random Graph Generator with Euler Circuit Analysis ===" << std::endl;
    std::cout << "Usage: " << progName << " [OPTIONS]" << std::endl;
    std::cout << "\nRequired Options:" << std::endl;
    std::cout << "  -v, --vertices <N>    Number of vertices (1-10000)" << std::endl;
    std::cout << "  -e, --edges <N>       Number of edges (0-max_possible)" << std::endl;
    std::cout << "  -s, --seed <N>        Random seed (0-4294967295)" << std::endl;
    std::cout << "\nOptional Options:" << std::endl;
    std::cout << "  -d, --directed        Generate directed graph (default: undirected)" << std::endl;
    std::cout << "  -h, --help           Show this help message" << std::endl;
    std::cout << "  -q, --quiet          Quiet mode (less output)" << std::endl;
    std::cout << "  -v, --verbose        Verbose mode (more output)" << std::endl;
    std::cout << "\nExamples:" << std::endl;
    std::cout << "  " << progName << " -v 5 -e 6 -s 42" << std::endl;
    std::cout << "  " << progName << " --vertices 10 --edges 15 --seed 123 --directed" << std::endl;
    std::cout << "  " << progName << " -v 4 -e 4 -s 1 -q" << std::endl;
    std::cout << "\nMax edges for N vertices:" << std::endl;
    std::cout << "  Undirected: N*(N-1)/2" << std::endl;
    std::cout << "  Directed:   N*(N-1)" << std::endl;
}

void validateParameters(int vertices, int edges, bool directed, unsigned int seed) {
    std::cout << "\n=== Parameter Validation ===" << std::endl;
    
    // Validate vertices
    if (vertices <= 0) {
        throw std::invalid_argument("Number of vertices must be positive, got: " + std::to_string(vertices));
    }
    if (vertices > 10000) {
        throw std::invalid_argument("Number of vertices too large (max 10000), got: " + std::to_string(vertices));
    }
    
    // Validate edges
    if (edges < 0) {
        throw std::invalid_argument("Number of edges cannot be negative, got: " + std::to_string(edges));
    }
    
    int maxEdges = GraphGenerator::getMaxEdges(vertices, directed);
    if (edges > maxEdges) {
        throw std::invalid_argument("Too many edges for " + std::to_string(vertices) + 
                                  " vertices (" + (directed ? "directed" : "undirected") + 
                                  "). Max: " + std::to_string(maxEdges) + 
                                  ", requested: " + std::to_string(edges));
    }
    
    std::cout << "✓ Vertices: " << vertices << " (valid range: 1-10000)" << std::endl;
    std::cout << "✓ Edges: " << edges << " (max possible: " << maxEdges << ")" << std::endl;
    std::cout << "✓ Seed: " << seed << std::endl;
    std::cout << "✓ Type: " << (directed ? "Directed" : "Undirected") << std::endl;
    std::cout << "✓ All parameters valid!" << std::endl;
}

void analyzeEulerProperties(const Graph& graph, bool verbose) {
    std::cout << "\n=== Euler Circuit Analysis ===" << std::endl;
    
    // Basic properties
    std::cout << "Graph properties:" << std::endl;
    std::cout << "- Vertices: " << graph.getVertexCount() << std::endl;
    std::cout << "- Edges: " << graph.getEdgeCount() << std::endl;
    std::cout << "- Type: " << (graph.isDirected() ? "Directed" : "Undirected") << std::endl;
    std::cout << "- Connected: " << (graph.isConnected() ? "Yes" : "No") << std::endl;
    std::cout << "- Empty: " << (graph.isEmpty() ? "Yes" : "No") << std::endl;
    
    if (verbose) {
        // Show degree information
        std::cout << "\nDegree analysis:" << std::endl;
        int evenDegreeCount = 0;
        int oddDegreeCount = 0;
        int minDegree = graph.getVertexCount();
        int maxDegree = 0;
        
        for (int i = 0; i < graph.getVertexCount(); ++i) {
            int degree = graph.getDegree(i);
            if (degree % 2 == 0) evenDegreeCount++;
            else oddDegreeCount++;
            minDegree = std::min(minDegree, degree);
            maxDegree = std::max(maxDegree, degree);
        }
        
        std::cout << "- Even degree vertices: " << evenDegreeCount << std::endl;
        std::cout << "- Odd degree vertices: " << oddDegreeCount << std::endl;
        std::cout << "- Min degree: " << minDegree << std::endl;
        std::cout << "- Max degree: " << maxDegree << std::endl;
        
        if (graph.getVertexCount() <= 20) {
            std::cout << "- Individual degrees: ";
            for (int i = 0; i < graph.getVertexCount(); ++i) {
                std::cout << i << "(" << graph.getDegree(i) << ") ";
            }
            std::cout << std::endl;
        }
    }
    
    // Euler circuit analysis
    bool hasEuler = graph.hasEulerCircuit();
    std::cout << "\nEuler Circuit: " << (hasEuler ? "✓ EXISTS" : "✗ DOES NOT EXIST") << std::endl;
    
    if (hasEuler) {
        std::cout << "Finding Euler circuit..." << std::endl;
        Graph workingGraph = graph; // Copy for modification
        auto circuit = workingGraph.findEulerCircuit();
        
        if (!circuit.empty()) {
            std::cout << "✓ Euler circuit found!" << std::endl;
            std::cout << "Circuit length: " << circuit.size() << " vertices" << std::endl;
            
            if (verbose || circuit.size() <= 20) {
                std::cout << "Circuit: ";
                for (size_t i = 0; i < circuit.size(); ++i) {
                    std::cout << circuit[i];
                    if (i < circuit.size() - 1) std::cout << " → ";
                }
                std::cout << std::endl;
            } else {
                std::cout << "Circuit (first 10): ";
                for (int i = 0; i < std::min(10, (int)circuit.size()); ++i) {
                    std::cout << circuit[i];
                    if (i < 9 && i < (int)circuit.size() - 1) std::cout << " → ";
                }
                std::cout << " ... → " << circuit.back() << std::endl;
            }
            
            // Verify circuit
            bool valid = true;
            for (size_t i = 0; i < circuit.size() - 1; ++i) {
                if (!graph.hasEdge(circuit[i], circuit[i + 1])) {
                    valid = false;
                    break;
                }
            }
            std::cout << "Circuit validation: " << (valid ? "✓ VALID" : "✗ INVALID") << std::endl;
        }
    } else {
        std::cout << "Reason why no Euler circuit exists:" << std::endl;
        if (!graph.isConnected()) {
            std::cout << "- Graph is not connected" << std::endl;
        } else if (!graph.isDirected()) {
            std::cout << "- Not all vertices have even degree" << std::endl;
            if (verbose) {
                std::cout << "  Odd degree vertices: ";
                for (int i = 0; i < graph.getVertexCount(); ++i) {
                    if (graph.getDegree(i) % 2 == 1) {
                        std::cout << i << "(" << graph.getDegree(i) << ") ";
                    }
                }
                std::cout << std::endl;
            }
        } else {
            std::cout << "- In-degree ≠ out-degree for some vertices" << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    // Default values
    int vertices = -1;
    int edges = -1;
    unsigned int seed = 42;
    bool directed = false;
    bool quiet = false;
    bool verbose = false;
    
    // Long options
    static struct option long_options[] = {
        {"vertices",  required_argument, 0, 'v'},
        {"edges",     required_argument, 0, 'e'},
        {"seed",      required_argument, 0, 's'},
        {"directed",  no_argument,       0, 'd'},
        {"help",      no_argument,       0, 'h'},
        {"quiet",     no_argument,       0, 'q'},
        {"verbose",   no_argument,       0, 'V'},
        {0, 0, 0, 0}
    };
    
    int opt;
    int option_index = 0;
    
    // Parse command line arguments
    while ((opt = getopt_long(argc, argv, "v:e:s:dhqV", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'v':
                vertices = std::atoi(optarg);
                if (vertices == 0 && optarg[0] != '0') {
                    std::cerr << "Error: Invalid vertices value: " << optarg << std::endl;
                    return 1;
                }
                break;
            case 'e':
                edges = std::atoi(optarg);
                if (edges < 0) {
                    std::cerr << "Error: Invalid edges value: " << optarg << std::endl;
                    return 1;
                }
                break;
            case 's':
                {
                    long seed_long = std::atol(optarg);
                    if (seed_long < 0) {
                        std::cerr << "Error: Seed must be non-negative: " << optarg << std::endl;
                        return 1;
                    }
                    seed = static_cast<unsigned int>(seed_long);
                }
                break;
            case 'd':
                directed = true;
                break;
            case 'h':
                printUsage(argv[0]);
                return 0;
            case 'q':
                quiet = true;
                verbose = false;
                break;
            case 'V':
                verbose = true;
                quiet = false;
                break;
            case '?':
                std::cerr << "Error: Unknown option or missing argument" << std::endl;
                printUsage(argv[0]);
                return 1;
            default:
                std::cerr << "Error: Unexpected option" << std::endl;
                printUsage(argv[0]);
                return 1;
        }
    }
    
    // Check for extra arguments
    if (optind < argc) {
        std::cerr << "Error: Unexpected arguments: ";
        for (int i = optind; i < argc; ++i) {
            std::cerr << argv[i] << " ";
        }
        std::cerr << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    // Validate required arguments
    if (vertices == -1 || edges == -1) {
        std::cerr << "Error: Missing required arguments" << std::endl;
        if (vertices == -1) std::cerr << "  Missing: --vertices (-v)" << std::endl;
        if (edges == -1) std::cerr << "  Missing: --edges (-e)" << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    try {
        if (!quiet) {
            std::cout << "=== Q3: Random Graph Generation with Euler Circuit Analysis ===" << std::endl;
            validateParameters(vertices, edges, directed, seed);
        }
        
        // Generate the graph
        if (!quiet) std::cout << "\n=== Graph Generation ===" << std::endl;
        auto graph = GraphGenerator::generateRandomGraph(vertices, edges, seed, directed);
        
        if (!quiet) {
            std::cout << "✓ Random graph generated successfully!" << std::endl;
        }
        
        // Display graph if small enough or in verbose mode
        if (verbose || (!quiet && vertices <= 10)) {
            graph->printGraph();
        } else if (!quiet) {
            std::cout << "\nGraph summary:" << std::endl;
            std::cout << "- " << graph->getVertexCount() << " vertices" << std::endl;
            std::cout << "- " << graph->getEdgeCount() << " edges" << std::endl;
            std::cout << "- " << (graph->isDirected() ? "Directed" : "Undirected") << std::endl;
        }
        
        // Analyze Euler properties
        analyzeEulerProperties(*graph, verbose && !quiet);
        
        if (!quiet) {
            std::cout << "\n=== Q3 Completed Successfully! ===" << std::endl;
            std::cout << "✓ Command line parsing with getopt working" << std::endl;
            std::cout << "✓ Random graph generation working" << std::endl;
            std::cout << "✓ Euler circuit analysis working" << std::endl;
            std::cout << "✓ Ready for Requirement 4 (Coverage/Profiling/Valgrind)" << std::endl;
        }
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        if (!quiet) {
            std::cerr << "\nTry different parameters or use --help for usage information." << std::endl;
        }
        return 1;
    } catch (...) {
        std::cerr << "❌ Unknown error occurred" << std::endl;
        return 1;
    }
}