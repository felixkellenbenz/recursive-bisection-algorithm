#pragma once

#include <utility>

#include "graph.hpp"

namespace compress {

class BiPartitioner {
 public:
  virtual ~BiPartitioner() {}
  virtual std::pair<VertexSet, VertexSet> bisect(const QDGraph& toBisect) = 0;
};

class RandomBiPartioner : public BiPartitioner {
 private:
 public:
  std::pair<VertexSet, VertexSet> bisect(const QDGraph& toBisect) override;
};

}  // namespace compress
