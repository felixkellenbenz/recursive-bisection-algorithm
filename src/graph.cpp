#include <cstddef>
#include <string>

#include "graph.hpp"
#include "exception.hpp"
#include "utility.hpp"

namespace compress {

bool checkVertexTypeInSet(const VertexSet& checkSet, Vertex::Type expectedType) {
  for (auto& vertex : checkSet) {
    if (vertex.vertexType != expectedType)
      return false;
  }

  return true;
}

std::string vertexTypeToString(const Vertex::Type& type) {
  switch(type) {
    case Vertex::Type::NONE: 
      return "NONE";
    case Vertex::Type::DATA:
      return "DATA";
    case Vertex::Type::QUERY:
      return "QUERY";
  }
}

bool hasEdge(const Edge& searchedEdge, const Graph& toSearchIn)  {
  auto NeighborChecker = [&](const Vertex& start, const Vertex& end) {
    auto lookup = toSearchIn.neighbours(start);

    if (!lookup.size()) return false;

    for (const auto& neighbourVertex : lookup)
      if (neighbourVertex == end) return true;

     return false;
  };

  bool firstToSecond = NeighborChecker(searchedEdge.first, searchedEdge.second); 
  bool secondToFirst = NeighborChecker(searchedEdge.second, searchedEdge.first);
  bool foundEdge = toSearchIn.isDirected() ? firstToSecond : firstToSecond && secondToFirst;

  return foundEdge;
}

[[nodiscard]] VertexSet Graph::vertices() const {
  VertexSet vertices;

  for (auto& [vertex, neighbors] : adjacencyList)
    vertices.insert(vertex); 

  return vertices;
}

std::list<Vertex> Graph::neighbours(const Vertex& v) const {
  if (adjacencyList.find(v) != adjacencyList.end())
    return adjacencyList.find(v)->second;

  return std::list<Vertex>{}; 
}

[[nodiscard]] std::size_t Graph::size() const {
  std::size_t graphSize = 0;
  
  for (auto& [vertex, neighbours] : adjacencyList)
    graphSize += neighbours.size();

  return graphSize >> 1;
}

bool Graph::addEdge(const Edge& newEdge) {
  if (hasEdge(newEdge, *this)) return false;

  adjacencyList[newEdge.first].push_back(newEdge.second);
  if (!directed)
    adjacencyList[newEdge.second].push_back(newEdge.first);

  return true;
}

// Something with parsing a qdgraph does not work
// Somehow edges get multiplied
QDGraph::QDGraph(const Graph& constructFrom) 
  : Graph(), queryVertexSet(), dataVertexSet() {
   
  for (auto& [vertex, neighbors] : constructFrom) {
    Vertex queryVertex{vertex.vertexID, Vertex::Type::QUERY};
    Vertex dataVertex{vertex.vertexID, Vertex::Type::DATA};
    for (auto& neighbor : neighbors) {
      Vertex neighborAsQuery{neighbor.vertexID, Vertex::Type::QUERY};
      Vertex neighborAsData{neighbor.vertexID, Vertex::Type::DATA};

      this->adjacencyList[queryVertex].push_back(neighborAsData);
      this->adjacencyList[dataVertex].push_back(neighborAsQuery);
    }
    
    queryVertexSet.insert(queryVertex);
    dataVertexSet.insert(dataVertex);
  } 
}

QDGraph::QDGraph(const VertexSet& _queryVertexSet, const VertexSet& _dataVertexSet, const QDGraph& constructFrom)
  : queryVertexSet(_queryVertexSet), dataVertexSet(_dataVertexSet) {
  
  if (!checkVertexTypeInSet(queryVertexSet, Vertex::Type::QUERY) 
      || !checkVertexTypeInSet(dataVertexSet, Vertex::Type::DATA)) {
    throw WrongVertexTypeException{"There is a vertex in a set that does not have the required type"};
  }
  
  for (auto& [vertex, neighbors] : constructFrom) {
    for (auto& neighbor : neighbors) {
       if ((queryVertexSet.contains(vertex) && dataVertexSet.contains(neighbor)) 
          || (queryVertexSet.contains(neighbor) && dataVertexSet.contains(vertex))) 
       this->adjacencyList[vertex].push_back(neighbor); 
    } 
  }
}

[[nodiscard]] VertexSet QDGraph::vertices() const {
  VertexSet vertices;

  for (auto& vertex : queryVertexSet)
    vertices.insert(vertex);

  for (auto& vertex : dataVertexSet)
    vertices.insert(vertex);
   
  return vertices;
}

VertexSet QDGraph::dataVertices() const {
  return dataVertexSet;
}

VertexSet QDGraph::queryVertices() const {
  return queryVertexSet;
}

bool QDGraph::addEdge(const Edge& newEdge) {
  return true;
}
}  // namespace compress
