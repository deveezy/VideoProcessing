cmake_minimum_required(VERSION 3.14)

project(VideoProcessing)
set(CMAKE_CXX_STANDARD 17)

include(cmake/Development.cmake)

add_executable(${CMAKE_PROJECT_NAME}
	src/main.cpp
	src/load_frame.cpp
	src/load_frame.hpp
)

add_subdirectory(lib/FFmpeg)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glfw3 REQUIRED)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

target_link_libraries(${CMAKE_PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} glfw FFmpeg)