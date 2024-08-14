#pragma once

#include <utility>
#include <memory>

#include "graph.hpp"

namespace compress {

class BiPartitioner {
 public:
  virtual ~BiPartitioner() {}
  virtual std::pair<VertexSet, VertexSet> bisect(const QDGraph& toBisect) = 0;
  virtual std::unique_ptr<BiPartitioner> clone() = 0;
};

class RandomBiPartioner : public BiPartitioner {
 private:
 public:
  std::pair<VertexSet, VertexSet> bisect(const QDGraph& toBisect) override;
  std::unique_ptr<BiPartitioner> clone() override;
};

}  // namespace compress
