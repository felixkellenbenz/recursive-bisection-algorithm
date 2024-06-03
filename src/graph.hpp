#pragma once

#include <cstddef>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace compress {

typedef long Vertex;
typedef std::pair<Vertex, Vertex> Edge;
typedef std::unordered_set<Vertex> VertexSet;
typedef std::unordered_map<Vertex, long> Order;

class Graph {
 protected:
  mutable std::unordered_map<Vertex, std::list<Vertex>> adjacencyList;
  std::size_t numberOfEdges;

  bool hasEdge(const Edge& searchedEdge) const;

 public:
  typedef std::unordered_map<Vertex, std::list<Vertex>>::iterator iterator;

  Graph() : adjacencyList(), numberOfEdges(0) {}

  bool addEdge(const Edge& newEdge);

  std::vector<Vertex> vertices() const;
  std::list<Vertex> neighbours(const Vertex& v) const;

  std::size_t virtual order() const { return adjacencyList.size(); }
  std::size_t virtual size() const { return numberOfEdges; }

  iterator begin() const { return adjacencyList.begin(); };
  iterator end() const { return adjacencyList.end(); }

  std::string toString() const;
};

// improve QD Graph model
class QDGraph : public Graph {
 private:
  VertexSet queryVertices;
  VertexSet dataVertices;

 public:
  QDGraph() : Graph(), queryVertices(), dataVertices() {}
  QDGraph(VertexSet queryVertices, VertexSet dataVertices, const Graph& original);

  std::size_t order() const override { return dataVertices.size() + queryVertices.size();}
  std::size_t size() const override { return numberOfEdges * 2; }

  std::size_t dataOrder() const { return dataVertices.size(); }
  std::size_t queryOrder() const { return queryVertices.size(); }

  VertexSet getDataVertices() const {return dataVertices; }
  VertexSet getQueryVertices() const {return queryVertices;};
};
}  // namespace compress