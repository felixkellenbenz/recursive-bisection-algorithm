#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

#include "algorithm.hpp"
#include "graph.hpp"

namespace compress {

namespace utility {

double inline numberOfBits(double x) { return std::floor(std::log2(x)) + 1; }

double inline computeGain(int neighboursInFirst, int neighboursInSecond,
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

void swapTwoVertices(VertexSet& first, VertexSet& second, Vertex inFirst,
                     Vertex inSecond) {
  first.erase(inFirst);
  second.erase(inSecond);
  second.insert(inFirst);
  first.insert(inSecond);
}

double computeMoveGain(Vertex toMove, VertexSet firstPart, VertexSet secondPart,
                       const QDGraph& toReorder) {
  auto adjacentVertices = toReorder.neighbours(toMove);

  VertexSet& moveFrom = firstPart.contains(toMove) ? firstPart : secondPart;
  VertexSet& moveTo = firstPart.contains(toMove) ? secondPart : firstPart;

  double sum = 0;

  for (const Vertex& adjacentVertex : adjacentVertices) {
    // compute cost function without moving
    int neighboursInMoveFrom = countNeighboursInSet(
        adjacentVertex, moveFrom, toReorder.neighbours(adjacentVertex));
    int neighboursInMoveTo = countNeighboursInSet(
        adjacentVertex, moveTo, toReorder.neighbours(adjacentVertex));

    int originalCost = computeGain(neighboursInMoveFrom, neighboursInMoveTo,
                                   moveFrom.size(), moveTo.size());

    // compute cost function after moving
    // Can we avoid actually moving the vertices ?
    moveFrom.erase(toMove);
    moveTo.insert(toMove);

    neighboursInMoveFrom = countNeighboursInSet(
        adjacentVertex, moveFrom, toReorder.neighbours(adjacentVertex));
    neighboursInMoveTo = countNeighboursInSet(
        adjacentVertex, moveTo, toReorder.neighbours(adjacentVertex));

    int costAfterMoving = computeGain(neighboursInMoveFrom, neighboursInMoveTo,
                                      moveFrom.size(), moveTo.size());

    moveTo.erase(toMove);
    moveFrom.insert(toMove);

    sum += originalCost - costAfterMoving;
  }

  return sum;
}

} // namespace utils

std::pair<VertexSet, VertexSet> Reorderer::bisect(std::pair<VertexSet, VertexSet> partition,
                                                  const QDGraph& toReorder) {
  bool swapped = true;
  long graphSize = partition.first.size() + partition.second.size();

  for (long i = 0; swapped && i < 20; i++) {
    swapped = false;

    auto MoveGainsArrayCreator = [&](const VertexSet& createFrom, bool secondCall) {
      std::vector<std::pair<Vertex, double>> moveGainsArray;

      auto createFromIter = createFrom.begin();

      for (long i = 0;
           i < createFrom.size() && createFromIter != createFrom.end(); i++) {
        // Consider iter limit when logging
        actionLogger.logComputingGainsForith(secondCall ? i + (graphSize - createFrom.size()) : i, graphSize, loggingEnabeled);
        double moveGain =
            utility::computeMoveGain(*createFromIter, partition.first, partition.second, toReorder);
        moveGainsArray.push_back({*createFromIter, moveGain});
      }

      return moveGainsArray;
    };

    std::vector<std::pair<Vertex, double>> moveGainsForFirstPart =
        MoveGainsArrayCreator(partition.first, false);
    std::vector<std::pair<Vertex, double>> moveGainsForSecondPart =
        MoveGainsArrayCreator(partition.second, true);

    auto comperator = [](const std::pair<Vertex, double>& lhs,
                         const std::pair<Vertex, double>& rhs) {
      return lhs.second > rhs.second;
    };

    std::sort(moveGainsForFirstPart.begin(), moveGainsForFirstPart.end(),
              comperator);
    std::sort(moveGainsForSecondPart.begin(), moveGainsForSecondPart.end(),
              comperator);

    auto firstPartIt = moveGainsForFirstPart.begin();
    auto secPartIt = moveGainsForSecondPart.begin();

    for (; firstPartIt != moveGainsForFirstPart.end() &&
           secPartIt != moveGainsForSecondPart.end();) {
      if (firstPartIt->second + secPartIt->second > 0) {
        utility::swapTwoVertices(partition.first, partition.second, firstPartIt->first, secPartIt->first);
        swapped = true;
        firstPartIt++;
        secPartIt++;
      } else {
        break;
      }
    }
  }

  return partition;
}

Order Reorderer::reorder(const QDGraph& toReorder, long begin, long end) {
  Order vertexOrder;
  actionLogger.logReorderingSubgraph(toReorder.dataOrder(), loggingEnabeled);
  auto partition = partitionStrategy->bisect(toReorder);
  bool recursionEnd = false;

  auto OrderCombiner = [&](const Order& toAdd) {
    for (auto& [vertex, orderValue] : toAdd) {
      vertexOrder[vertex] = orderValue;
    }
  };

  if (toReorder.dataOrder() == 1) {
    auto dataVertices = toReorder.getDataVertices();
    vertexOrder[*dataVertices.begin()] = begin;
    recursionEnd = true;
  }

  if (!recursionEnd) {
    partition = bisect(partition, toReorder);

    // calculate suborders and glue them together
    long upper = begin + partition.first.size();

    QDGraph firstSubgraph(toReorder.getQueryVertices(), partition.first,
                          toReorder);
    QDGraph secondSubgraph(toReorder.getQueryVertices(), partition.second,
                           toReorder);

    Order suborderOfFirst = reorder(firstSubgraph, begin, upper - 1);
    Order suborderOfSecond = reorder(secondSubgraph, upper, end);

    OrderCombiner(suborderOfFirst);
    OrderCombiner(suborderOfSecond);
  }

  return vertexOrder;
}

}  // namespace compress
