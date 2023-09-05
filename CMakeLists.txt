cmake_minimum_required(VERSION 3.9)
project(skiplist LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR})

add_library(skiplist SHARED)
set_target_properties(skiplist PROPERTIES LINKER_LANGUAGE CXX)
target_sources(skiplist
  PRIVATE
    "skiplist.h"
)

add_subdirectory("third_party/googletest")

add_executable(skiplist_tests "")
target_sources(skiplist_tests
  PRIVATE
    "skiplist_test.cc"
)

target_link_libraries(
  skiplist_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(skiplist_tests)

add_subdirectory("third_party/benchmark")

add_executable(skiplist_benchmark "")
target_sources(skiplist_benchmark
  PRIVATE
    "benchmarks/skiplist_benchmark.cc"
)

target_link_libraries(
  skiplist_benchmark
  benchmark::benchmark
)
