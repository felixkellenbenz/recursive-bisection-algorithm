#include <algorithm>
#include <cmath>
#include <memory>
#include <random>
#include <vector>

#include "partitioner.hpp"
#include "graph.hpp"

namespace compress {

std::pair<VertexSet, VertexSet> RandomBiPartioner::bisect(
    const QDGraph& toBisect) {
  VertexSet firstSet = {};
  VertexSet secondSet = {};
  long upperboundFloored = toBisect.numberOfDataVertices() >> 1;

  std::random_device seed;
  std::default_random_engine engine(seed());

  auto AsVertexVector = [](VertexSet&& toVector) {
    std::vector<Vertex> vertexVector;

    for (auto& vertex : toVector) vertexVector.push_back(vertex);

    return vertexVector;
  };

  std::vector<Vertex> vertices = AsVertexVector(toBisect.dataVertices());

  std::shuffle(vertices.begin(), vertices.end(), engine);

  for (long i = 0; i < upperboundFloored; ++i)
    firstSet.insert(std::move(vertices[i]));

  for (long i = upperboundFloored; i < vertices.size(); ++i)
    secondSet.insert(std::move(vertices[i]));

  return {firstSet, secondSet};
}

std::unique_ptr<BiPartitioner> RandomBiPartioner::clone() {
  return std::make_unique<RandomBiPartioner>();
}

}  // namespace compress
