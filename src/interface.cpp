#include <cctype>
#include <filesystem>
#include <memory>
#include <string>
#include <algorithm>

#include "interface.hpp"
#include "partitioner.hpp"
#include "exception.hpp"


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
      lastConfiguration.partitioningStrategy = std::unique_ptr<BiPartitioner>{nullptr};
  }
  
  Configuration CLIArgumentParser::parseConfiguration() {
    
    for (std::size_t i = 0; i < arguments.size(); i ++) {
      auto argument = arguments[i];


      if (argument == "--strategy" || argument == "-s") {

        if (i == arguments.size() - 1) {
          throw ParsingException("The --strategy or -s flag needs an argument");
        }

        auto value = arguments[++i];

        if (value.empty() || value[0] == '-') {
          throw ParsingException("The --strategy or -s flag needs an argument");
        } 


        parsePartioningStrategyFromString(value);
      }
      else if (argument == "--rec-depth" || argument == "-r") {

      
        if (i == arguments.size() - 1) {
          throw ParsingException("The --rec-depth or -r flag requires an argument"); 
        }

        auto value = arguments[++i];

        if (value.find_first_not_of(numberChars) != std::string_view::npos) {
          throw ParsingException("The --rec-depth or -r flag requires a number as an argument");
        }

        lastConfiguration.recursionDepth = std::stoi(value);

      }
      else if (argument == "--path" || argument == "-p") {

        if (i == arguments.size() - 1) {
          throw ParsingException("The --path or -p flag requires an argument");
        }

        auto value = arguments[++i];
        
        std::filesystem::path asPath{value};

        if (!asPath.has_filename()) {
          throw ParsingException("The path that was given to --path or -p does not point to a file");
        }
        
        lastConfiguration.graphPath = asPath;
      } else {
         throw ParsingException("The option " + argument + " was not recognized");
      }
    }

    return lastConfiguration;
  }


}
