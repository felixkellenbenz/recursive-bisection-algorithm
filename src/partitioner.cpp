#include "partitioner.hpp"

#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

#include "graph.hpp"

namespace compress {

std::pair<VertexSet, VertexSet> RandomBiPartioner::bisect(
    const QDGraph& toBisect) {
  VertexSet firstSet = {};
  VertexSet secondSet = {};
  long upperboundFloored = toBisect.dataOrder() >> 1;

  std::random_device seed;
  std::default_random_engine engine(seed());

  auto AsVertexVector = [](VertexSet&& toVector) {
    std::vector<Vertex> vertexVector;

    for (auto& vertex : toVector)
      vertexVector.push_back(vertex);

    return vertexVector;
  };

  std::vector<Vertex> vertecies = AsVertexVector(toBisect.getDataVertices());

  std::shuffle(vertecies.begin(), vertecies.end(), engine);

  for (long i = 0; i < upperboundFloored; ++i)
    firstSet.insert(std::move(vertecies[i]));

  for (long i = upperboundFloored; i < vertecies.size(); ++i)
    secondSet.insert(std::move(vertecies[i]));

  return {firstSet, secondSet};
}
}  // namespace compress
