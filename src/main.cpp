#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "algorithm.hpp"
#include "graph.hpp"
#include "parser.hpp"
#include "partitioner.hpp"
#include "interface.hpp"

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

int main(int argc, char** argv) {
  
  // TODO: include seperation characters as program arguments
  compress::GraphParser parser('#', ' ');
  std::vector<std::string> arguements;
 
  for (int i = 1; i < argc; i++) 
    arguements.push_back(std::string{argv[i]});

  try {
    compress::CLIArgumentParser argumentParser{arguements}; 
    compress::Configuration config = argumentParser.parseConfiguration(); 

    compress::Graph graph = parser.parseFromFile(config.graphPath);
    compress::QDGraph qd(graph);


    compress::Reorderer reorderer(std::make_unique<compress::RandomBiPartioner>());
    auto vertexOrder = reorderer.reorder(qd, 1, qd.numberOfDataVertices());

  } catch (std::exception e) {
    std::cout << "Error: " << e.what() << std::endl;
  }

}
