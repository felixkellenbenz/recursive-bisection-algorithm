#pragma once

#include <utility>

#include "graph.hpp"
#include "partitioner.hpp"

namespace compress {

class Reorderer {
 private:
  BiPartitioner& partitioningStrategy;
  int maxRecursionDepth;
  unsigned int currentDepth;

 public:
  explicit Reorderer(BiPartitioner& _partitionStrategy, int _maxRecursionDepth = -1) :
    partitioningStrategy(_partitionStrategy), maxRecursionDepth(_maxRecursionDepth), currentDepth(0) {}

  Order reorder(const QDGraph& toReorder, long begin, long end);
  std::pair<VertexSet, VertexSet> bisect(std::pair<VertexSet, VertexSet>,
                                         const QDGraph& toReorder);
};
}  // namespace compress
