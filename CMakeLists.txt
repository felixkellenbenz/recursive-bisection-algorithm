cmake_minimum_required(VERSION 3.28)

project(bpa)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(external)
add_subdirectory(test)

add_executable (
  bpa
  src/main.cpp  
)

target_link_libraries(bpa PUBLIC bpasrclib)

if (CMAKE_BUILD_TYPE EQUAL DEBUG)
  target_compile_options(bpa PUBLIC "-g")
endif (CMAKE_BUILD_TYPE EQUAL DEBUG)
