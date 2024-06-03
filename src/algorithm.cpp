
#include "algorithm.hpp"

#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

#include "graph.hpp"

namespace compress {

double inline numberOfBits(double x) { return std::floor(std::log2(x)) + 1; }

int countNeighboursInSet(Vertex toCountFor, const VertexSet& countIn, const std::list<Vertex>& neighbours) {

  int numberOfNeighbours = 0;
  for (const Vertex& neighbour : neighbours) {
    numberOfNeighbours = countIn.contains(neighbour) ? ++numberOfNeighbours : numberOfNeighbours;
  }
  return numberOfNeighbours;
}

double computeMoveGain(const Vertex& toMove, VertexSet firstPart,
                                  VertexSet secondPart,
                                  const QDGraph& toReorder) {
  std::list<Vertex> neighbours = toReorder.neighbours(toMove);

  VertexSet& toMoveFrom = firstPart.contains(toMove) ? firstPart : secondPart;
  VertexSet& toMoveTo = firstPart.contains(toMove) ? secondPart : firstPart;

  double sum = 0;

  for (const Vertex& neighbour : neighbours) {
    // compute cost function without moving the vertex

    int neigboursInMoveFrom = countNeighboursInSet(neighbour, toMoveFrom, toReorder.neighbours(neighbour));
    int neigboursInMoveTo = countNeighboursInSet(neighbour, toMoveTo, toReorder.neighbours(neighbour));

    auto ComputeGainFunc = [&]() {
      return neigboursInMoveFrom *
                 numberOfBits(static_cast<double>(toMoveFrom.size()) /
                              (neigboursInMoveFrom + 1)) +
             neigboursInMoveTo *
                 numberOfBits(static_cast<double>(toMoveTo.size()) /
                              (neigboursInMoveTo + 1));
    };

    double originalCost = ComputeGainFunc();

    // compute cost function after moving the vertex
    toMoveFrom.erase(toMove);
    toMoveTo.insert(toMove);

    neigboursInMoveFrom = countNeighboursInSet(neighbour, toMoveFrom, toReorder.neighbours(neighbour));
    neigboursInMoveTo = countNeighboursInSet(neighbour, toMoveTo, toReorder.neighbours(neighbour));

    double costAfterMoving = ComputeGainFunc();

    toMoveFrom.insert(toMove);
    toMoveTo.erase(toMove);

    sum += originalCost - costAfterMoving;
  }

  return sum;
}

std::pair<VertexSet, VertexSet> bisect(VertexSet first, VertexSet second, const QDGraph& toReorder) {
  bool swapped = true;

  for (; swapped;) {
    swapped = false;

    auto MoveGainsArrayCreator = [=](const VertexSet& createFrom) {
      std::vector<std::pair<Vertex, double>> moveGainsArray;

      for (const Vertex& toMove : createFrom) {
        double moveGain = computeMoveGain(toMove, first, second, toReorder);
        moveGainsArray.push_back({toMove, moveGain});
      }

      return moveGainsArray;
    };

    std::vector<std::pair<Vertex, double>> moveGainsForFirstPart =
        MoveGainsArrayCreator(first);
    std::vector<std::pair<Vertex, double>> moveGainsForSecondPart =
        MoveGainsArrayCreator(second);

    auto comperator = [](std::pair<Vertex,double> lhs,
                         std::pair<Vertex, double> rhs) {
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
        first.erase(firstPartIt->first);
        second.erase(secPartIt->first);
        second.insert(secPartIt->first);
        first.insert(firstPartIt->first);
        swapped = true;
        firstPartIt++;
        secPartIt++;
      } else {
        break;
      }
    }
  }

  return {first, second}; 
}

Order Reorderer::reorder(const QDGraph& toReorder, long begin, long end) {
  Order vertexOrder;
  auto partition = partitionStrategy->bisect(toReorder);
  bool swapped = true;
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
    partition = bisect(partition.first, partition.second, toReorder);


    // calculate suborders and glue them together
    long upper = begin + partition.first.size() - 1;

    QDGraph firstSubgraph(toReorder.getQueryVertices(), partition.first, toReorder);
    QDGraph secondSubgraph(toReorder.getQueryVertices(), partition.second, toReorder);
    
    Order suborderOfFirst = reorder(firstSubgraph, begin, upper);
    Order suborderOfSecond = reorder(secondSubgraph, upper + 1, end);

    OrderCombiner(suborderOfFirst);
    OrderCombiner(suborderOfSecond);
  }

  return vertexOrder;
}
}  // namespace compress
