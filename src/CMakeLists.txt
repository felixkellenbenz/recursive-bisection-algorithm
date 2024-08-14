add_executable(
  bpalgo
  main.cpp
  graph.cpp
  parser.cpp
  partitioner.cpp
  algorithm.cpp 
  interface.cpp
)

add_library(
  bpalgolib
  partitioner.cpp
  partitioner.hpp
  algorithm.cpp 
  algorithm.hpp
  graph.cpp
  graph.hpp
  parser.cpp
  parser.hpp
  interface.hpp
  interface.cpp
)

set_target_properties(bpalgolib PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(bpalgolib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(bpalgo PUBLIC bpalgolib)
