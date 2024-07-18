#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "algorithm.hpp"
#include "graph.hpp"
#include "parser.hpp"
#include "partitioner.hpp"
#include "utility.hpp"

namespace compress {

double calculateBiMLogACost(Order vertexOrder, const Graph& toCalculateFor) {
  std::unordered_map<Vertex, std::vector<std::pair<Vertex, long>>>
      adjacencyListWithOrder;

  for (auto& [vertex, neighbours] : toCalculateFor) {
    adjacencyListWithOrder[vertex] =
        std::vector<std::pair<compress::Vertex, long>>();
    for (auto& neihgbour : neighbours) {
      adjacencyListWithOrder[vertex].push_back(
          {neihgbour, vertexOrder[neihgbour]});
    }
  }

  double BiMLogACost = 0;
  long gaps = 0;

  for (auto& [vertex, neighbours] : adjacencyListWithOrder) {

    if (vertex.vertexType != Vertex::Type::QUERY) 
      continue;

    std::sort(neighbours.begin(), neighbours.end(),
              [](std::pair<compress::Vertex, long>& lhs,
                 std::pair<compress::Vertex, long>& rhs) {
                return lhs.second < rhs.second;
              });

    for (long k = 0; k < neighbours.size() - 1; k++) {
      gaps += 1;
      BiMLogACost += std::floor(std::log2(neighbours[k + 1].second -
                                          neighbours[k].second)) + 1;
    }
  }

  return BiMLogACost / (gaps + adjacencyListWithOrder.size());
}

bool verifyOrder(Order vertexOrder) {
  std::unordered_set<long> seenOrderValues;
  std::vector<long> duplicates;
  bool valid = true;

  for (auto& [vertex, orderVal] : vertexOrder) {
    if (seenOrderValues.contains(orderVal)) {
      duplicates.push_back(orderVal);
      valid = false;
    }

    seenOrderValues.insert(orderVal);
  }

  return valid;
}

}  // namespace compress

int main() {
  compress::GraphParser parser('#', '\t');

  auto begin = std::chrono::steady_clock::now();
  compress::Graph graph = parser.parseFromFile("../graphs/sample_graph_2.txt");
  auto end = std::chrono::steady_clock::now();

  compress::QDGraph qd(graph);
  compress::Reorderer reorderer(std::make_unique<compress::RandomBiPartioner>());
  auto vertexOrder = reorderer.reorder(qd, 1, qd.numberOfDataVertices());

  // how to compute compression cost
  std::cout << "\nOrder is valid: " << compress::verifyOrder(vertexOrder) << '\n';
  std::cout << "BiMLogACost: "
            << compress::calculateBiMLogACost(vertexOrder, qd) << '\n';
}
