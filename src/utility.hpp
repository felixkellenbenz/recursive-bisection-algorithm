#pragma once

#include <unordered_map>
#include <unordered_set>
#include <cstdint>

namespace compress {

struct Vertex {

  enum class Type : std::uint8_t {
    NONE,
    QUERY,
    DATA
  };

  long vertexID;
  Type vertexType;

  Vertex(long _vertexID) : vertexID(_vertexID), vertexType(Type::NONE) {}
  Vertex(long _vertexID, Type _vertexType) : vertexID(_vertexID), vertexType(_vertexType) {}
};

bool operator==(const Vertex&, const Vertex&);


typedef std::pair<Vertex, Vertex> Edge;
typedef std::unordered_set<Vertex> VertexSet;
typedef std::unordered_map<Vertex, long> Order;

}
