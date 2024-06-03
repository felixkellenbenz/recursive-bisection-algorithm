
#include <gtest/gtest.h>
#include <memory>

#include "algorithm.hpp"
#include "graph.hpp"
#include "parser.hpp"
#include "partitioner.hpp"


TEST(ReorderTests, ReorderSample1) {
  compress::GraphParser parser('#', ' ');
  compress::Graph sampleGraph1 = parser.parseFromFile("/home/felix/Documents/University/Semester 4/Proseminar/Code/test/sample_graph_1.txt");
  compress::Reorderer reorder(std::make_unique<compress::RandomBiPartioner>()); 

  auto ToVertexSet = [](const std::vector<compress::Vertex> vertexVector) {
    compress::VertexSet newVertexSet;

    for (auto& vertex : vertexVector)
      newVertexSet.insert(vertex);

    return newVertexSet;
  };

  compress::QDGraph constructed(ToVertexSet(sampleGraph1.vertices()), ToVertexSet(sampleGraph1.vertices()), sampleGraph1);
  compress::Order vertexOrder = reorder.reorder(constructed, 1, constructed.dataOrder());

  ASSERT_EQ(vertexOrder.size(), constructed.dataOrder());
}