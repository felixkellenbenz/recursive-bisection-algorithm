#include <iostream>
#include <string_view>

#include "logger.hpp"

namespace compress {

bool CLILogger::log(std::string_view toLog, bool loggingEnabeled) {
  if (loggingEnabeled) std::cout << toLog << "\n";

  return loggingEnabeled;
}

bool CLILogger::logComputingGainsForith(long i, long total, bool loggingEnabeled) {
  std::string progressBar = "";

  long tenth = total / 50;
  
  for (int j = 0; j < 50; j++) {
    if (i >= (j * tenth)) {
       progressBar += "="; 
     }
  }

  std::string padding = "";

  for (int j = 0; j < (50 - progressBar.length()); j++) {
    padding += "-";   
  } 
  
  std::cout << '\r' << "[" << progressBar << padding << "]" << std::flush;

  return true;
}

bool CLILogger::logReorderingSubgraph(long numberOfVertices,
                                      bool loggingEnabeled) {
  std::cout << "\nReordering Subgraph with: " << numberOfVertices << " vertices\n";

  return true;
}

}  // namespace compress
