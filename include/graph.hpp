#pragma once

#include <cstddef>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <cstddef>

namespace compress {

struct Vertex {

  enum class Type {
    NONE,
    QUERY,
    DATA
  };

  long vertexID;
  Type  vertexType;

  Vertex(long _vertexID) : vertexID(_vertexID), vertexType(Type::NONE) {}
  Vertex(long _vertexID, Type _vertexType) : vertexID(_vertexID), vertexType(_vertexType) {}

};

bool operator==(const Vertex&, const Vertex&);

typedef std::pair<Vertex, Vertex> Edge;
typedef std::unordered_set<Vertex> VertexSet;
typedef std::unordered_map<Vertex, long> Order;

} // namespace compress

// maybe use boost hashcombine
template<>
struct std::hash<compress::Vertex> {
  
  std::size_t operator()(const compress::Vertex& vertex) const {
    std::size_t IDHash = std::hash<long>{}(vertex.vertexID);
    std::size_t TypeHash = std::hash<compress::Vertex::Type>{}(vertex.vertexType); 
    std::size_t firstCombine =  0 ^ IDHash + 0x9e3779b9 + (1 << 6) + (1 >> 2);
    return firstCombine ^ (TypeHash + 0x9e3779b9 + (1 << 6) + (1 >> 2));
  }
};


namespace compress {

class Graph {
 protected:
  mutable std::unordered_map<Vertex, std::list<Vertex>> adjacencyList;
  bool directed;

 public:
  typedef std::unordered_map<Vertex, std::list<Vertex>>::iterator iterator;

  explicit Graph(bool _directed = false) : adjacencyList(), directed(_directed) {} 
  virtual ~Graph() noexcept {} 

  [[nodiscard]] VertexSet virtual vertices() const;
  std::list<Vertex> neighbours(const Vertex& v) const;

  std::size_t virtual order() const { return adjacencyList.size(); }
  [[nodiscard]] std::size_t size() const;

  bool virtual addEdge(const Edge& newEdge);
  bool isDirected() const { return directed; }

  iterator begin() const { return adjacencyList.begin(); };
  iterator end() const { return adjacencyList.end(); }
};

class QDGraph : public Graph {
private:
  VertexSet queryVertexSet;
  VertexSet dataVertexSet;

public: 
  QDGraph() : queryVertexSet(), dataVertexSet() {}
  explicit QDGraph(const Graph&);
  QDGraph(const VertexSet&, const VertexSet&, const QDGraph&);
  ~QDGraph() noexcept {}
 
  VertexSet vertices() const override;
  [[nodiscard]] std::size_t order() const override { return queryVertexSet.size() + dataVertexSet.size(); }

  VertexSet dataVertices() const;
  VertexSet queryVertices() const;
  std::size_t numberOfDataVertices() const { return dataVertexSet.size(); }
  std::size_t numberOfQueryVertices() const { return queryVertexSet.size(); } 

  bool addEdge(const Edge& newEdge) override; 
};

}  // namespace compress
