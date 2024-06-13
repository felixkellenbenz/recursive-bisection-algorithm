#include <gtest/gtest.h>
#include "graph.hpp"
#include "parser.hpp"

TEST(GraphTests, parseSampleGraph1) {
  compress::GraphParser parser('#', ' ');
  compress::Graph sampleGraph1 = parser.parseFromFile("../graphs/sample_graph_1.txt");

  ASSERT_EQ(sampleGraph1.order(), 7L);
  ASSERT_EQ(sampleGraph1.size(), 9);
}

TEST(GraphTests, parseSampleGraph2) {
  compress::GraphParser parser;
  compress::Graph sampleGraph2 = parser.parseFromFile("../graphs/sample_graph_2.txt");

  ASSERT_EQ(sampleGraph2.order(), 36692L);
  ASSERT_EQ(sampleGraph2.size(), 183831L);
}

TEST(GraphTests, QDGraphConstrucionEmptySample1) {
  compress::GraphParser parser('#', ' ');
  compress::Graph sampleGraph1 = parser.parseFromFile("../graphs/sample_graph_1.txt");

  ASSERT_TRUE(sampleGraph1.order() != 0);
  ASSERT_TRUE(sampleGraph1.size() != 0);

  compress::QDGraph constructed({}, {}, sampleGraph1);

  ASSERT_EQ(constructed.order(), 0);
  ASSERT_EQ(constructed.size(), 0); 
}

TEST(GraphTests, QDConstructionThreeVerticesSample1) {
  compress::GraphParser parser('#', ' ');
  compress::Graph sampleGraph1 = parser.parseFromFile("../graphs/sample_graph_1.txt");

  ASSERT_TRUE(sampleGraph1.order() != 0);
  ASSERT_TRUE(sampleGraph1.size() != 0);

  auto ToVertexSet = [](const std::vector<compress::Vertex> vertexVector) {
    compress::VertexSet newVertexSet;

    for (auto& vertex : vertexVector)
      newVertexSet.insert(vertex);

    return newVertexSet;
  };

  compress::QDGraph constructed(ToVertexSet(sampleGraph1.vertices()), {1, 2, 3}, sampleGraph1);

  ASSERT_EQ(constructed.dataOrder(), 3);
  ASSERT_EQ(constructed.queryOrder(), 7);
  ASSERT_EQ(constructed.neighbours(1).size(), 2);
  ASSERT_EQ(constructed.neighbours(2).size(), 2);
  ASSERT_EQ(constructed.neighbours(3).size(), 2);
  ASSERT_EQ(constructed.size(), 6);
}

TEST(GraphTests, QDConstructionAllVertices) {
  compress::GraphParser parser('#', ' ');
  compress::Graph sampleGraph1 = parser.parseFromFile("../graphs/sample_graph_1.txt");

  ASSERT_TRUE(sampleGraph1.order() != 0);
  ASSERT_TRUE(sampleGraph1.size() != 0);

  auto ToVertexSet = [](const std::vector<compress::Vertex> vertexVector) {
    compress::VertexSet newVertexSet;

    for (auto& vertex : vertexVector)
      newVertexSet.insert(vertex);

    return newVertexSet;
  };

  compress::QDGraph constructed(ToVertexSet(sampleGraph1.vertices()), ToVertexSet(sampleGraph1.vertices()), sampleGraph1);

  ASSERT_EQ(constructed.dataOrder(), 7);
  ASSERT_EQ(constructed.queryOrder(), 7);
  ASSERT_EQ(constructed.order(), 14);
  ASSERT_EQ(constructed.size(), 18);
}
