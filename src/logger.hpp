#pragma once

#include <string_view>

namespace compress {

class Logger {
public:
  Logger() = default;
  Logger(const Logger&) = default;
  Logger(Logger&&) = default;
  virtual ~Logger() { }; 
  
  Logger& operator=(const Logger&) = default;
  Logger& operator=(Logger&&) = default;

  virtual bool log(std::string_view toLog, bool loggingEnabeled) = 0;
  virtual bool logComputingGainsForith(long i, bool loggingEnabeled) = 0;
  virtual bool logReorderingSubgraph(long numberOfVertices,  bool loggingEnabeled) = 0;
};

class CLILogger : public Logger {
private:
public:
  CLILogger() = default;
  CLILogger(const CLILogger&) = default;
  CLILogger(CLILogger&&) = default;  
  ~CLILogger() override { }; 

  CLILogger& operator=(const CLILogger&) = default;
  CLILogger& operator=(CLILogger&&) = default; 

  bool log(std::string_view toLog, bool loggingEnabeled) override;
  bool logComputingGainsForith(long i, bool loggingEnabeled) override;
  bool logReorderingSubgraph(long numberOfVertices,  bool loggingEnabeled) override;
};


} // namespace compress
