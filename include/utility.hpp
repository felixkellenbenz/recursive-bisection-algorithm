#pragma once

#include <cmath>
#include <cstdlib>

#include "graph.hpp"
#include "interface.hpp"

namespace compress {
namespace utility {

double calculateBiMLogACost(const Order& vertexOrder, const QDGraph& toCalculateFor);
bool verifyOrder(const Order& vertexOrder);
double numberOfBits(double x);
double computeGain(int neighboursInFirst, int neighboursInSecond, long sizeOfFirst, long sizeOfSecond);
int countNeighboursInSet(Vertex toCountFor, const VertexSet& countIn, const std::list<Vertex>& neighbours);
void swapTwoVertices(VertexSet& first, VertexSet& second, const Vertex& inFirst, const Vertex& inSecond);
double computeMoveGain(Vertex toMove, const VertexSet& firstPart, VertexSet secondPart, const QDGraph& toReorder);
bool validateConfiguration(const Configuration& config);


} // namespace utility
} // namesapce compress
