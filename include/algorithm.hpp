#pragma once

#include <memory>
#include <utility>

#include "graph.hpp"
#include "partitioner.hpp"
#include "exception.hpp"

namespace compress {

class Reorderer {
 private:
  std::unique_ptr<BiPartitioner> partitionStrategy;

 public:
  explicit Reorderer(std::unique_ptr<BiPartitioner> _partitionStrategy)
      : partitionStrategy(std::move(_partitionStrategy)) { 
        if (!partitionStrategy) {
          throw new NullPointerException(""); 
        } 
      }

  Order reorder(const QDGraph& toReorder, long begin, long end);
  std::pair<VertexSet, VertexSet> bisect(std::pair<VertexSet, VertexSet>,
                                         const QDGraph& toReorder);
};
}  // namespace compress
