#include <cmath>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <vector>

#include "algorithm.hpp"
#include "graph.hpp"
#include "parser.hpp"
#include "partitioner.hpp"
#include "interface.hpp"

int main(int argc, char** argv) {
  
  // TODO: include seperation characters as program arguments
  compress::GraphParser parser('#', ' ');
  std::vector<std::string> arguements;
 
  for (int i = 1; i < argc; i++) 
    arguements.push_back(std::string{argv[i]});

  try {
    compress::CLIArgumentParser argumentParser{arguements}; 
    compress::Configuration config = argumentParser.parseConfiguration(); 

    compress::Graph graph = parser.parseFromFile(config.graphPath);
    compress::QDGraph qd(graph);


    compress::Reorderer reorderer(std::make_unique<compress::RandomBiPartioner>());
    auto vertexOrder = reorderer.reorder(qd, 1, qd.numberOfDataVertices());

  } catch (std::exception e) {
    std::cout << "Error: " << e.what() << std::endl;
  }

}
