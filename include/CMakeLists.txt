add_library(
  bpalgolib
  graph.hpp
  parser.hpp
  partitioner.hpp
  algorithm.hpp
  interface.hpp
)

set_target_properties(bpalgolib PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(bpalgolib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")
