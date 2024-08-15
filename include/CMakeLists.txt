add_library(
  bpaheaderlib
  graph.hpp
  parser.hpp
  partitioner.hpp
  algorithm.hpp
  interface.hpp
  utility.hpp
)

set_target_properties(bpaheaderlib PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(bpaheaderlib PUBLIC "~/Documents/Code/BP/include")
