#pragma once

#include <stdexcept>

namespace compress {

class ParsingException : public std::runtime_error {
 public:
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

class NullPointerException : public std::runtime_error {
public:
  NullPointerException(const std::string& msg) : std::runtime_error(msg) {}
};
}  // namespace compress
