add_executable(
  bpalgo
  main.cpp
  graph.cpp
  parser.cpp
  partitioner.cpp
  algorithm.cpp 
  interface.cpp
)


target_link_libraries(bpalgo PUBLIC bpalgolib)
