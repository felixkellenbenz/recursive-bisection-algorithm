
#include <string_view>
#include <iostream>

#include "logger.hpp"

namespace compress {

bool CLILogger::log(std::string_view toLog, bool loggingEnabeled) {
  if (loggingEnabeled)
     std::cout << toLog << "\n";

  return loggingEnabeled;
}

bool CLILogger::logComputingGainsForith(long i, bool loggingEnabeled) {
  std::string ending =  "th";

  if (i == 1)
    ending = "st";
  else if (i == 2)
    ending = "nd";
  else if (i == 3)
    ending = "rd";

  std::cout << "Computing MoveGains for " << i << ending << "Vertex in current Set\n"; 

  return true;
}

bool CLILogger::logReorderingSubgraph(long numberOfVertices,  bool loggingEnabeled) {
  std::cout << "Reordering Subgraph of order " << numberOfVertices << '\n';

  return true;
}

} // namespace compress
