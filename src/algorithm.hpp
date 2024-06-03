#pragma once

#include <memory>

#include "graph.hpp"
#include "partitioner.hpp"

namespace compress {

class Reorderer {
 private:
  std::unique_ptr<BiPartitioner> partitionStrategy;

 public:
  Reorderer(std::unique_ptr<BiPartitioner> _partitionStrategy) : 
    partitionStrategy(std::move(_partitionStrategy)) {}
  Order reorder(const QDGraph& toReorder, long begin, long end);
};
}  // namespace compress