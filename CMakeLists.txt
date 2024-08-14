cmake_minimum_required(VERSION 3.28)

project(bpalgo)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(external)
add_subdirectory(test)

if (CMAKE_BUILD_TYPE EQUAL DEBUG)
  target_compile_options(bpalgo PUBLIC "-g")
endif (CMAKE_BUILD_TYPE EQUAL DEBUG)
