#pragma once

#include <stdexcept>

namespace compress {

class ParsingException : public std::runtime_error {
 public:
  enum class errortype {};
  ParsingException(const std::string& msg) : std::runtime_error(msg) {}
};

class VertexNotFoundException : public std::runtime_error {
public:
  VertexNotFoundException(const std::string& msg) : std::runtime_error(msg) {}
};

class WrongVertexTypeException : public std::runtime_error {
public:
  WrongVertexTypeException (const std::string& msg) : std::runtime_error(msg) {}
};

}  // namespace compress
