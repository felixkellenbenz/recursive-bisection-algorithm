#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>

#include "graph.hpp"
#include "interface.hpp"

namespace compress {
namespace utility {

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

double numberOfBits(double x) { return std::floor(std::log2(x)) + 1; }

double computeGain(int neighboursInFirst, int neighboursInSecond,
                          long sizeOfFirst, long sizeOfSecond) {
  return neighboursInFirst *
             numberOfBits(static_cast<double>(sizeOfFirst) / neighboursInFirst +
                          1) +
         neighboursInSecond *
             numberOfBits(
                 static_cast<double>(sizeOfSecond) / neighboursInSecond + 1);
}

int countNeighboursInSet(Vertex toCountFor, const VertexSet& countIn,
                         const std::list<Vertex>& neighbours) {
  int numberOfNeighbours = 0;
  for (const Vertex& neighbour : neighbours) {
    numberOfNeighbours =
        countIn.contains(neighbour) ? ++numberOfNeighbours : numberOfNeighbours;
  }

  return numberOfNeighbours;
}

void swapTwoVertices(VertexSet& first, VertexSet& second, const Vertex& inFirst,
                     const Vertex& inSecond) {
  first.erase(inFirst);
  second.erase(inSecond);
  second.insert(inFirst);
  first.insert(inSecond);
}

double computeMoveGain(Vertex toMove, const VertexSet& firstPart, VertexSet secondPart,
                       const QDGraph& toReorder) {
  auto adjacentVertices = toReorder.neighbours(toMove);

  const VertexSet& moveFrom = firstPart.contains(toMove) ? firstPart : secondPart;
  const VertexSet& moveTo = firstPart.contains(toMove) ? secondPart : firstPart;

  double sum = 0;

  for (const Vertex& adjacentVertex : adjacentVertices) {

    // compute cost function without moving
    int neighboursInMoveFrom = countNeighboursInSet(
        adjacentVertex, moveFrom, toReorder.neighbours(adjacentVertex));
    int neighboursInMoveTo = countNeighboursInSet(
        adjacentVertex, moveTo, toReorder.neighbours(adjacentVertex));

    int originalCost = computeGain(neighboursInMoveFrom, neighboursInMoveTo,
                                   moveFrom.size(), moveTo.size());

    long sizeOfMoveFromAfterMoving = moveFrom.size() - 1;
    long sizeOfMoveToAfterMoving = moveTo.size() + 1;

    // toMove is a neighbour of the adjacentVertex so moving 
    // to move from one set to another can be calculate by subtracting and adding 
    // to the already known number of neighbours
    neighboursInMoveFrom--;
    neighboursInMoveTo++;

    int costAfterMoving = computeGain(neighboursInMoveFrom, neighboursInMoveTo,
                                      sizeOfMoveFromAfterMoving, sizeOfMoveToAfterMoving);

    sum += originalCost - costAfterMoving;
  }

  return sum;
}

bool validateConfiguration(const Configuration& config) {
 
  if (!config.partitioningStrategy)
    return false;

  if (!std::ifstream{config.graphPath}.good())
    return false;

  return true;
}

} // namespace utility
} // namespace compress
