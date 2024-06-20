#pragma once

#include <memory>
#include <utility>

#include "graph.hpp"
#include "logger.hpp"
#include "partitioner.hpp"

namespace compress {

class Reorderer {
 private:
  std::unique_ptr<BiPartitioner> partitionStrategy;
  Logger& actionLogger;
  bool loggingEnabeled;

 public:
  Reorderer(std::unique_ptr<BiPartitioner> _partitionStrategy,
            Logger& _actionLogger)
      : partitionStrategy(std::move(_partitionStrategy)),
        actionLogger(_actionLogger),
        loggingEnabeled(true) {}

  Order reorder(const QDGraph& toReorder, long begin, long end);
  std::pair<VertexSet, VertexSet> bisect(std::pair<VertexSet, VertexSet>,
                                         const QDGraph& toReorder);
};
}  // namespace compress
