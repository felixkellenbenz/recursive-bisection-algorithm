#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "algorithm.hpp"
#include "graph.hpp"
#include "parser.hpp"
#include "partitioner.hpp"
#include "utility.hpp"

namespace compress {

double calculateBiMLogACost(const Order& vertexOrder, const QDGraph& toCalculateFor) {
  std::unordered_map<Vertex, std::vector<std::pair<Vertex, long>>>
      adjacencyListWithOrder;


  for (auto& vertex : toCalculateFor.queryVertices()) { 
    for (auto& neighbor : toCalculateFor.neighbours(vertex)) {
      adjacencyListWithOrder[vertex].push_back({neighbor, vertexOrder.at(neighbor)});
    }
  }

  double BiMLogACost = 0;
  long gaps = 0;

  for (auto& [vertex, neighbours] : adjacencyListWithOrder) {

    std::sort(neighbours.begin(), neighbours.end(),
              [](std::pair<compress::Vertex, long>& lhs,
                 std::pair<compress::Vertex, long>& rhs) {
                return lhs.second < rhs.second;
              });

    for (long k = 0; k < neighbours.size() - 1; k++) {
      gaps += 1;
      double difference = neighbours[k + 1].second - neighbours[k].second;
      BiMLogACost += std::floor(std::log2(difference) + 1);
    }
  }

  return BiMLogACost / gaps;
}

bool verifyOrder(const Order& vertexOrder) {
  std::unordered_set<long> seenOrderValues;
  bool valid = true;

  for (auto& [vertex, orderVal] : vertexOrder) {
    if (seenOrderValues.contains(orderVal)) {
      return false;
    }
  }

  return true;
}

}  // namespace compress

int main() {
  compress::GraphParser parser('#', ' ');

  auto begin = std::chrono::steady_clock::now();
  compress::Graph graph = parser.parseFromFile("../graphs/sample_graph_5.txt");
  auto end = std::chrono::steady_clock::now();

  compress::QDGraph qd(graph);
  compress::Reorderer reorderer(std::make_unique<compress::RandomBiPartioner>());
  auto vertexOrder = reorderer.reorder(qd, 1, qd.numberOfDataVertices());

  // how to compute compression cost
  std::cout << "\nOrder is valid: " << compress::verifyOrder(vertexOrder) << '\n';
  std::cout << "BiMLogACost: "
            << compress::calculateBiMLogACost(vertexOrder, qd) << '\n';
}
