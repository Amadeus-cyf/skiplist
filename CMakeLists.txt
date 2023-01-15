cmake_minimum_required(VERSION 3.9)
project(skiplist LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)

add_library(skiplist SHARED)
set_target_properties(skiplist PROPERTIES LINKER_LANGUAGE CXX)
target_sources(skiplist
  PRIVATE
    "src/skiplist.h"
)

add_subdirectory("third_party/googletest")

add_executable(skiplist_test "")
target_sources(skiplist_test
  PRIVATE
    "src/skiplist_test.cc"
)

target_link_libraries(
  skiplist_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(skiplist_test)
