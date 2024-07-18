#include "graph.hpp"
#include "utility.hpp"

#include <gtest/gtest.h>

TEST(conversionTests, converGraphToQueryData) {
  compress::Vertex v1{1, compress::Vertex::Type::NONE};
  compress::Vertex v2{2, compress::Vertex::Type::NONE};
  compress::Vertex v3{3, compress::Vertex::Type::NONE};

  compress::Graph graph; 

  graph.addEdge({v1, v2});
  graph.addEdge({v2, v3});

  compress::QDGraph qdGraph{graph};

  ASSERT_EQ(qdGraph.numberOfDataVertices(), 3);
  ASSERT_EQ(qdGraph.numberOfQueryVertices(), 3);
 
}
