add_executable(
  bpalgo
  main.cpp
  graph.cpp
  parser.cpp
  partitioner.cpp
  algorithm.cpp 
  utility.cpp
  interface.cpp
)

add_library(
  bpalgolib
  graph.hpp
  parser.hpp
  partitioner.hpp
  algorithm.hpp
  graph.cpp
  parser.cpp
  partitioner.cpp
  algorithm.cpp 
  utility.hpp
  utility.cpp
  interface.hpp
  interface.cpp
)

set_target_properties(bpalgolib PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(bpalgolib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(bpalgo PUBLIC bpalgolib)
