
#include "utility.hpp"

namespace compress {

bool operator==(const Vertex& lhs, const Vertex& rhs) {
  return lhs.vertexID == rhs.vertexID && lhs.vertexType == rhs.vertexType;
}

}
