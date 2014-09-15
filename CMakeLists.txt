cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(quad-tree2)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

#SET (CMAKE_BUILD_TYPE "Debug")

if(CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()



find_package(OpenGL REQUIRED)

find_package(Qt4 COMPONENTS QtOpenGL QtCore QtGui REQUIRED)
include(${QT_USE_FILE})

find_package(Boost REQUIRED)
include_directories(include ${Boost_INCLUDE_DIR})


add_subdirectory(include)
add_subdirectory(sources)
add_subdirectory(tests)
add_subdirectory(examples)

