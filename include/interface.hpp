#pragma once

#include <filesystem>
#include <memory>
#include <vector>
#include <string_view>

#include "partitioner.hpp"

namespace compress  {

struct Configuration { 

  std::unique_ptr<BiPartitioner> partitioningStrategy = std::make_unique<RandomBiPartioner>();
  std::filesystem::path graphPath = std::filesystem::current_path();
  char commentCharacter = '#';
  char vertexSeparator = ' ';
  int recursionDepth = -1; 

  Configuration() {}
 
  // deep copy polymorphic type using clone() method.
  // do we even need to deep copy them they have no data ?
  Configuration(const Configuration& rhs) : partitioningStrategy(rhs.partitioningStrategy->clone()),
                                            graphPath(rhs.graphPath), recursionDepth(rhs.recursionDepth) {}
};

class CLIArgumentParser {
  private:
    Configuration lastConfiguration;
    std::vector<std::string> arguments;
    int argumentsProcessed;
    static constexpr std::string_view numberChars = "0123456789";

    void parsePartioningStrategyFromString(std::string parseFrom);

  public:
    CLIArgumentParser() : lastConfiguration(), arguments(), argumentsProcessed(0) {}
    explicit CLIArgumentParser(std::vector<std::string> _arguments) : lastConfiguration(),
                            arguments(std::move(_arguments)), argumentsProcessed(arguments.size()) {} 

    Configuration parseConfiguration();
 };


} // namespace compress
