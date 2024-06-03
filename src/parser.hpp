#pragma once

#include <filesystem>

#include "graph.hpp"

namespace compress {

class GraphParser {
 private:
  const char commentCharacter;
  const char vertexSeperator;

 public:
  GraphParser(char _commentCharacter = '#', char _vertexSeperator = '\t')
      : commentCharacter(_commentCharacter),
        vertexSeperator(_vertexSeperator) {}

  [[nodiscard]] Graph parseFromFile(
      const std::filesystem::path& filePath) const;
};

}  // namespace compress