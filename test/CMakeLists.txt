if (BUILD_TESTING)
  add_executable(
    bpalgotests
    graph_tests.cpp)

  target_link_libraries(bpalgotests PRIVATE GTest::gtest_main)
  target_link_libraries(bpalgotests PUBLIC bpalgolib)

  include(GoogleTest)
  gtest_discover_tests(bpalgotests)
endif()
