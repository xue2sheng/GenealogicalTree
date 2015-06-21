/// \file main.cpp

#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "version.h"

using namespace boost;

/// \brief  Main function
/// \param  argc An integer argument count of the command line arguments
/// \param  argv An argument vector of the command line arguments
/// \return an integer 0 upon exit success  
///
/// @startuml ../image/main.png
/// Alice -> Bob: trace 
/// @enduml
/// \image latex main.png width=100px
///
///
int main(int argc, char** argv)
{
    // create a typedef for the Graph type
    typedef adjacency_list<vecS, vecS, bidirectionalS> Graph;

    // Make convenient labels for the vertices
    enum { A, B, C, D, E, N };
    const int num_vertices = N;
    const char* name = "ABCDE";

    // writing out the edges in the graph
    typedef std::pair<int, int> Edge;
    Edge edge_array[] = 
    { Edge(A,B), Edge(A,D), Edge(C,A), Edge(D,C),
      Edge(C,E), Edge(B,D), Edge(D,E) };
    const int num_edges = sizeof(edge_array)/sizeof(edge_array[0]);

    // declare a graph object
    Graph g(num_vertices);

    // add the edges to the graph object
    for (int i = 0; i < num_edges; ++i)
      add_edge(edge_array[i].first, edge_array[i].second, g);
    
    std::cout << "Hello Boost!, " << VERSION << std::endl;
    std::cout << GIT_DETAILS << std::endl;

    return 0;
}

