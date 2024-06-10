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
#include "logger.hpp"
#include "parser.hpp"
#include "partitioner.hpp"

namespace compress {
namespace mla {

std::vector<std::vector<int>> findCombos(int start) {
  std::vector<std::vector<int>> combinations;
  std::string s;

  for (int i = 1; i <= start; i++) {
    s += std::to_string(i);
  }

  do {
    std::vector<int> nextPermutation;
    for (char c : s) nextPermutation.push_back(c - 48);

    combinations.push_back(nextPermutation);
  } while (std::next_permutation(s.begin(), s.end()));

  return combinations;
}

compress::Order forceOrder(const compress::Graph& toOrder, bool islog = false) {
  std::vector<std::vector<int>> combinations = findCombos(toOrder.order());
  auto vertces = toOrder.vertices();
  compress::Order candidate;
  int min = 0xFFFFFF;

  for (auto& permutation : combinations) {
    compress::Order nextOrder;
    std::vector<compress::Edge> alreadySeen;
    for (int i = 0; i < toOrder.order(); i++) {
      nextOrder.insert({vertces[i], permutation[i]});
    }

    double sum = 0;
    for (auto& [vertex, neigbours] : toOrder) {
      for (auto& neigbour : neigbours) {
        bool seen = false;
        for (compress::Edge edge : alreadySeen) {
          compress::Edge compare = {vertex, neigbour};
          if (edge == compare) {
            seen = true;
            break;
          }
        }

        double intermediateResLog =
            std::log2(std::abs(nextOrder[vertex] - nextOrder[neigbour])) + 1;
        double intermediateRes =
            islog ? intermediateResLog
                  : std::abs(nextOrder[vertex] - nextOrder[neigbour]);
        sum += !seen ? intermediateRes : 0;

        if (!seen) {
          alreadySeen.push_back({vertex, neigbour});
          alreadySeen.push_back({neigbour, vertex});
        }
      }
    }

    candidate = sum < min ? nextOrder : candidate;
    min = sum < min ? sum : min;
  }

  return candidate;
}

}  // namespace mla

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
    std::sort(neighbours.begin(), neighbours.end(),
              [](std::pair<compress::Vertex, long>& lhs,
                 std::pair<compress::Vertex, long>& rhs) {
                return lhs.second < rhs.second;
              });

    for (long k = 0; k < neighbours.size() - 1; k++) {
      gaps++;
      BiMLogACost += std::floor(std::log2(neighbours[k + 1].second -
                                          neighbours[k].second)) +
                     1;
    }
  }

  return BiMLogACost / (gaps + adjacencyListWithOrder.size());
}

bool verifyOrder(Order vertexOrder) {
  std::unordered_set<long> seenOrderValues;
  std::vector<long> duplicates;
  bool valid = true;
  ;

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
  compress::CLILogger logger;
  compress::GraphParser parser('#', '\t');

  auto begin = std::chrono::steady_clock::now();
  compress::Graph graph = parser.parseFromFile(
      "/home/felix/Documents/University/Semester "
      "4/Proseminar/Code/test/sample_graph_2.txt");
  auto end = std::chrono::steady_clock::now();

  auto ToVertexSet = [&]() {
    compress::VertexSet vertexSet;

    for (auto& [vertex, neighbours] : graph) vertexSet.insert(vertex);

    return vertexSet;
  };

  compress::QDGraph qd(ToVertexSet(), ToVertexSet(), graph);
  compress::Reorderer reorderer(std::make_unique<compress::RandomBiPartioner>(),
                                logger);
  auto vertexOrder = reorderer.reorder(qd, 1, qd.getDataVertices().size());

  // how to compute compression cost
  std::cout << "Order is valid: " << compress::verifyOrder(vertexOrder) << '\n';
  std::cout << "BiMLogACost: "
            << compress::calculateBiMLogACost(vertexOrder, qd) << '\n';
}
