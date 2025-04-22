#include <cctype>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <algorithm>

#include "interface.hpp"
#include "partitioner.hpp"
#include "exception.hpp"
#include "utility.hpp"

namespace compress  {

std::string stringToLowerCase(const std::string& toConvert) {
  auto converted= toConvert;

  std::transform(converted.begin(), converted.end(), converted.begin(),
      [](unsigned char c) { return std::tolower(c); });

  return converted;
}

void CLIArgumentParser::parsePartioningStrategyFromString(std::string parseFrom) {
  
  if (stringToLowerCase(parseFrom) == "random")
    lastConfiguration.partitioningStrategy = std::make_unique<RandomBiPartioner>();
  else if (stringToLowerCase(parseFrom) == "kahip")
    lastConfiguration.partitioningStrategy = std::make_unique<RandomBiPartioner>();
}

Configuration CLIArgumentParser::parseConfiguration() {
  
  for (std::size_t i = 0; i < arguments.size(); i ++) {
    auto argument = arguments[i];

    if (i == 0 && !argument.starts_with("-")) {

      std::filesystem::path asPath = std::filesystem::path{argument};
      lastConfiguration.graphPath = asPath;

      if (!std::ifstream{asPath}.good()) {
        throw ParsingException("The specified path does not point to a file");
      }

    } else if (argument == "--strategy" || argument == "-s") {

      if (i == arguments.size() - 1) {
        throw ParsingException("The --strategy or -s flag needs an argument");
      }

      auto value = arguments[++i];

      if (value.empty() || value[0] == '-') {
        throw ParsingException("The --strategy or -s flag needs an argument");
      } 


      parsePartioningStrategyFromString(value);
    } else if (argument == "--rec-depth" || argument == "-r") {
    
      if (i == arguments.size() - 1) {
        throw ParsingException("The --rec-depth or -r flag requires an argument"); 
      }

      auto value = arguments[++i];

      if (value.find_first_not_of(numberChars) != std::string_view::npos) {
        throw ParsingException("The --rec-depth or -r flag requires a number as an argument");
      }

      lastConfiguration.recursionDepth = std::stoi(value);

    } else if (argument == "--comment" || argument == "-c") {

      if (i == arguments.size() - 1) {
        throw ParsingException("The --comment or -c flag requires an argument");
      }
 
      auto value = arguments[++i];

      if (value.length() > 1 || value.length() == 0) {
        throw ParsingException("The --comment or -c flag exactly one character as argument");
      }

      lastConfiguration.commentCharacter = value[0];

    } else if (argument == "--vertex" || argument == "-v") {

      if (i == arguments.size() - 1) {
        throw ParsingException("The --vertex or -v flag requires an argument");
      }
 
      auto value = arguments[++i];

      if (value.length() > 1 || value.length() == 0) {
        throw ParsingException("The --vertex or -v flag exactly one character as argument");
      }

      lastConfiguration.vertexSeparator = value[0];

    } else {
       throw ParsingException("The option " + argument + " was not recognized");
    }
  }

  if (!utility::validateConfiguration(lastConfiguration))
    throw ParsingException("The graph path or the partition strategy was not specified"); 

  return lastConfiguration;
}


}
