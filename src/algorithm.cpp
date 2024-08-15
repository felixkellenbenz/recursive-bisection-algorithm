#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

#include "algorithm.hpp"
#include "graph.hpp"
#include "utility.hpp"

namespace compress {

std::pair<VertexSet, VertexSet> Reorderer::bisect(std::pair<VertexSet, VertexSet> partition,
                                                  const QDGraph& toReorder) {
  bool swapped = true;
  long graphSize = partition.first.size() + partition.second.size();

  for (long i = 0; swapped && i < 20; i++) {
    swapped = false;

    auto MoveGainsArrayCreator = [&](const VertexSet& createFrom, long indexOffset) {
      std::vector<std::pair<Vertex, double>> moveGainsArray;

      auto createFromIter = createFrom.begin();

      for (long i = 0;
           i < createFrom.size() && createFromIter != createFrom.end(); i++) {
        double moveGain =
            utility::computeMoveGain(*createFromIter, partition.first, partition.second, toReorder);
        moveGainsArray.push_back({*createFromIter, moveGain});
      }

      return moveGainsArray;
    };

    std::vector<std::pair<Vertex, double>> moveGainsForFirstPart =
        MoveGainsArrayCreator(partition.first, 0);
    std::vector<std::pair<Vertex, double>> moveGainsForSecondPart =
        MoveGainsArrayCreator(partition.second, partition.first.size());

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
           secPartIt != moveGainsForSecondPart.end();) { if (firstPartIt->second + secPartIt->second > 0) {
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
  auto partition = partitionStrategy->bisect(toReorder);
  bool recursionEnd = false;


  auto OrderCombiner = [&](const Order& toAdd) {
    for (auto& [vertex, orderValue] : toAdd) {
      vertexOrder[vertex] = orderValue;
    }
  };

  if (toReorder.numberOfDataVertices() == 1) {
    auto dataVertices = toReorder.dataVertices();
    vertexOrder[*dataVertices.begin()] = begin;
    recursionEnd = true;
  }

  if (!recursionEnd) {
    partition = bisect(partition, toReorder);

    long upper = begin + partition.first.size();

    QDGraph firstSubgraph(toReorder.queryVertices(), partition.first,
                          toReorder);
    QDGraph secondSubgraph(toReorder.queryVertices(), partition.second,
                           toReorder);

    Order suborderOfFirst = reorder(firstSubgraph, begin, upper - 1);
    Order suborderOfSecond = reorder(secondSubgraph, upper, end);

    OrderCombiner(suborderOfFirst);
    OrderCombiner(suborderOfSecond);
  }

  return vertexOrder;
}

}  // namespace compress
