cmake_minimum_required(VERSION 3.14)

if(CMAKE_GENERATOR MATCHES "Ninja")
	# Force update the program to allow updating the ninja version
	# Don't worry about bad builds
	# Ninja handles this update by starting over. Here is the error message ninja gave from
	# upgrading from version 1.7 -> 1.10
	# "ninja: warning: bad deps log signature or version; starting over"
	# This is why ninja is fantastic
	# set(CMAKE_MAKE_PROGRAM "C:/dev/utils/ninja.exe" CACHE FILEPATH "" FORCE)
endif()

project(VideoProcessing)
set(CMAKE_CXX_STANDARD 17)

message("generator is set to ${CMAKE_GENERATOR}")
include(cmake/Development.cmake)

add_executable(${CMAKE_PROJECT_NAME}
	src/main.cpp
	src/load_frame.cpp
	src/load_frame.hpp
)

add_subdirectory(lib)
add_subdirectory(lib/glfw)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glfw opengl32.lib glu32.lib)