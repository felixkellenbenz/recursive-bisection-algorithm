#pragma once

#include <memory>

#include "graph.hpp"
#include "partitioner.hpp"
#include "logger.hpp"

namespace compress {

class Reorderer {
 private:
  std::unique_ptr<BiPartitioner> partitionStrategy;
  Logger& actionLogger;
  bool loggingEnabeled;

 public:
  Reorderer(std::unique_ptr<BiPartitioner> _partitionStrategy, Logger& _actionLogger) : 
    partitionStrategy(std::move(_partitionStrategy)), actionLogger(_actionLogger),
    loggingEnabeled(true)  {}

  Order reorder(const QDGraph& toReorder, long begin, long end);
  std::pair<VertexSet, VertexSet> bisect(VertexSet first, VertexSet second, const QDGraph& toReorder);

  void enableLogging() {
    loggingEnabeled = true;
  }
  
  void disableLogging() {
    loggingEnabeled = false; 
  }
};
}  // namespace compress
