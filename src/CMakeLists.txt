add_executable(
  bpalgo
  main.cpp
  graph.cpp
  parser.cpp
  partitioner.cpp
  algorithm.cpp 
  logger.cpp
  utility.cpp
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
  logger.hpp
  logger.cpp
  utility.hpp
  utility.cpp
)

set_target_properties(bpalgolib PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(bpalgolib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(bpalgo PUBLIC bpalgolib)
