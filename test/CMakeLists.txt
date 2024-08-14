if (BUILD_TESTING)
  add_executable(
    bpatests
    graph_tests.cpp)

  target_link_libraries(bpatests PUBLIC GTest::gtest_main)
  target_link_libraries(bpatests PUBLIC bpasrclib)

  include(GoogleTest)
  gtest_discover_tests(bpatests)
endif()
