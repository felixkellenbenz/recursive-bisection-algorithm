#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "graph.hpp"
#include "parser.hpp"
#include "partitioner.hpp"

// How to transform graph ?

 
std::vector<std::vector<int>> findCombos(int start) {
  std::vector<std::vector<int>> combinations;
  std::string s;

  for (int i = 1; i <= start; i++) {
    s += std::to_string(i);
  }

  do {
    std::vector<int> nextPermutation;
    for (char c : s)
      nextPermutation.push_back(c - 48); 

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

        double intermediateResLog = std::log2(std::abs(nextOrder[vertex] - nextOrder[neigbour])) + 1;
        double intermediateRes = islog ? intermediateResLog : std::abs(nextOrder[vertex] - nextOrder[neigbour]); 
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

int main() {
  compress::GraphParser parser('#', ' ');

  auto begin = std::chrono::steady_clock::now();
  compress::Graph graph = parser.parseFromFile("../test.txt");
  auto end = std::chrono::steady_clock::now();

  std::cout << "Number of Veticies: " << graph.order() << " Number of Edges: " << graph.size() << "\n";
  auto partitioner = std::make_unique<compress::RandomBiPartioner>();
}