#pragma once

#include <string_view>

namespace compress {

class Logger {
 protected:
  bool loggingEnabeled;
 public:
  Logger() : loggingEnabeled(true) {}
  Logger(const Logger&) = default;
  Logger(Logger&&) = default;
  virtual ~Logger() {};

  Logger& operator=(const Logger&) = default;
  Logger& operator=(Logger&&) = default;

  virtual bool log(std::string_view toLog) = 0;
  virtual bool logComputingGainsForith(long i, long total) = 0;
  virtual bool logReorderingSubgraph(long numberOfVertices) = 0;

  void enableLogging() {
    loggingEnabeled = true;
  }
  
  void disableLogging() {
    loggingEnabeled = false;
  }
};

class CLILogger : public Logger {
 private:
   int statusLineLength;
   int loggingCap;
 public:
  CLILogger(int _statusLineLength = 50, int _loggingCap = -1) :
    Logger(), statusLineLength(_statusLineLength), loggingCap(_loggingCap) {}
  CLILogger(const CLILogger&) = default;
  CLILogger(CLILogger&&) = default;
  ~CLILogger() override {};

  CLILogger& operator=(const CLILogger&) = default;
  CLILogger& operator=(CLILogger&&) = default;

  bool log(std::string_view toLog) override;
  bool logComputingGainsForith(long i, long total) override;
  bool logReorderingSubgraph(long numberOfVertices) override;
  
  void checkLoggingCap(long total) {
    loggingEnabeled = total >= loggingCap;
  }
};

}  // namespace compress
