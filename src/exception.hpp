#pragma once

#include <stdexcept>

namespace compress {

class ParsingException : public std::runtime_error {
 public:
  enum class errortype {};
  ParsingException(const std::string& msg) : std::runtime_error(msg) {}
};

}  // namespace compress
