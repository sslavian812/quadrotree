cmake_minimum_required(VERSION 2.8)

project(quad-tree-test)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})


find_package(Boost COMPONENTS random REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARYDIR})

set(SOURCES compressed-quad-tree-test.cpp) # quad-tree-test.cpp skip-quad-tree-test.cpp rectangle-test.cpp)

add_executable(quad-tree-test ${SOURCES})
target_link_libraries(quad-tree-test ${GTEST_BOTH_LIBRARIES})


file(GLOB_RECURSE HEADERS "*.h")
add_custom_target(quad-tree-test_headers SOURCES ${HEADERS})
