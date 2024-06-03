#include "parser.hpp"

#include <cctype>
#include <filesystem>
#include <fstream>
#include <string>

#include "exception.hpp"
#include "graph.hpp"

namespace compress {

Graph GraphParser::parseFromFile(const std::filesystem::path& filePath) const {
  std::ifstream file{filePath};

  if (!file.good())
    throw ParsingException("The specified file could not be opened.");

  Graph parsingResult;

  int lineCount = 1;
  for (std::string line; file.good() && std::getline(file, line);) {
    if (line.starts_with(commentCharacter)) continue;

    auto sepPos = line.find(vertexSeperator);

    if (sepPos == std::string::npos)
      throw ParsingException("In line: " + std::to_string(lineCount) +
                             " there was no vertex seperator found");

    std::string fromNodeAsString = line.substr(0, sepPos);
    std::string toNodeAsString = line.substr(++sepPos);

    // handle case that there are strings in the input
    int fromNode = std::stoi(fromNodeAsString);
    int toNode = std::stoi(toNodeAsString);

    parsingResult.addEdge({fromNode, toNode});
    lineCount++;
  }

  return parsingResult;
}

}  // namespace compress
