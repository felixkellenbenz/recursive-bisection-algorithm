#include "graph.hpp"

#include <climits>
#include <sstream>
#include <utility>
#include <vector>

namespace compress {

bool Graph::hasEdge(const Edge& searchedEdge) const {
  auto firstVertexLookup = adjacencyList.find(searchedEdge.first);
  auto secondVertexLookup = adjacencyList.find(searchedEdge.second);

  // refactor with lamda
  bool firstToSecond = false;
  bool secondToFirst = false;

  if (firstVertexLookup == adjacencyList.end()) return false;

  for (const auto& neighbourVertex : firstVertexLookup->second)
    if (neighbourVertex == searchedEdge.second) return firstToSecond = true;

  if (secondVertexLookup == adjacencyList.end()) return false;

  for (const auto& neighbourVertex : secondVertexLookup->second)
    if (neighbourVertex == searchedEdge.first) return secondToFirst = true;

  return secondToFirst && firstToSecond;
}

bool Graph::addEdge(const Edge& newEdge) {
  if (hasEdge(newEdge)) return false;

  adjacencyList[newEdge.first].push_back(newEdge.second);
  adjacencyList[newEdge.second].push_back(newEdge.first);

  numberOfEdges += 1;
  return true;
}

std::list<Vertex> Graph::neighbours(const Vertex& v) const {
  return adjacencyList.find(v) != adjacencyList.end() ? adjacencyList[v]
                                                      : std::list<Vertex>();
}

std::string Graph::toString() const {
  std::stringstream builder;

  for (auto& [vertex, neigbours] : adjacencyList) {
    for (auto& neigbour : neigbours) {
      builder << vertex << " --> " << neigbour << '\n';
    }
  }

  return builder.str();
}

std::vector<Vertex> Graph::vertices() const {
  std::vector<Vertex> vertecies;

  for (auto& [vertex, neigbour] : adjacencyList) vertecies.push_back(vertex);

  return vertecies;
}

QDGraph::QDGraph(VertexSet _queryVertices, VertexSet _dataVertices, const Graph& original)
    : Graph(), queryVertices(std::move(_queryVertices)), dataVertices(std::move(_dataVertices)) {
  for (auto& [vertex, neigbours] : original) {
    for (auto& neigbour : neigbours) {
      if (dataVertices.contains(neigbour) && queryVertices.contains(neigbour) &&
          dataVertices.contains(vertex) && queryVertices.contains(vertex) && 
          !hasEdge({vertex, neigbour})) {
        adjacencyList[vertex].push_back(neigbour);
        adjacencyList[neigbour].push_back(vertex);        
        numberOfEdges += 1;
      }
    }
  }
}
}  // namespace compress
