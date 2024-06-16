#include <iostream>
#include <string_view>

#include "logger.hpp"

namespace compress {
 
bool CLILogger::log(std::string_view toLog) {
  if (loggingEnabeled) std::cout << toLog << "\n";

  return loggingEnabeled;
}

bool CLILogger::logComputingGainsForith(long i, long total) {
  std::string progressBar = "";

  if (total >= loggingCap)
    loggingEnabeled = true;

  if (!loggingEnabeled || total < loggingCap) {
    loggingEnabeled = false;
    return false;
  }

  long tenth = total / statusLineLength;
  
  for (int j = 0; j < statusLineLength; j++) {
    if (i >= (j * tenth)) {
       progressBar += "="; 
     }
  }

  std::string padding = "";

  for (int j = 0; j < (statusLineLength - progressBar.length()); j++) {
    padding += "-";   
  } 
  
  std::cout << '\r' << "[" << progressBar << padding << "]" << std::flush;

  return true;
}

bool CLILogger::logReorderingSubgraph(long numberOfVertices) {
  checkLoggingCap(numberOfVertices);  
  if (!loggingEnabeled)
    return false;

  std::cout << "\n\nReordering Subgraph with: " << numberOfVertices << " vertices";
  return true;
}

}  // namespace compress
