add_library(
  bpasrclib 
  graph.cpp
  parser.cpp
  partitioner.cpp
  algorithm.cpp 
  interface.cpp
  utility.cpp
)

target_link_libraries(bpasrclib PUBLIC bpaheaderlib)
