cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(quad-tree-example)

#add_executable(quad-tree quad-tree.cpp)
#target_link_libraries(quad-tree visualization)

add_executable(compressed-quad-tree compressed-quad-tree.cpp)
target_link_libraries(compressed-quad-tree visualization)

#add_executable(skip-quad-tree skip-quad-tree.cpp)
#target_link_libraries(skip-quad-tree visualization)

