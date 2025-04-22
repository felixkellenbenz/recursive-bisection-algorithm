#include <cmath> 
#include <cstdlib> 
#include <iostream>
#include <memory>
#include <vector>

#include "algorithm.hpp"
#include "exception.hpp"
#include "graph.hpp"
#include "parser.hpp"
#include "interface.hpp"

int main(int argc, char** argv) {  
  std::vector<std::string> arguements;

  if (argc <= 1) {
    std::cout << "Error: You need to specify a graph path\n";
    std::exit(1);
  }
 
  for (int i = 1; i < argc; i++) 
    arguements.push_back(std::string{argv[i]});

  compress::Configuration config;
  compress::Graph graph;

  try {
    compress::CLIArgumentParser argumentParser{arguements}; 
    config = argumentParser.parseConfiguration(); 

    compress::GraphParser parser(config.commentCharacter, config.vertexSeparator);
    graph = parser.parseFromFile(config.graphPath);

   } catch (compress::ParsingException e) {
    std::cout << "Error: " << e.what() << "\n";
    std::exit(1);
   }

  compress::QDGraph qd(graph);

  compress::Reorderer reorderer(*config.partitioningStrategy);
  auto vertexOrder = reorderer.reorder(qd, 1, qd.numberOfDataVertices());
}
