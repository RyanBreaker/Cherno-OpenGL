cmake_minimum_required(VERSION 3.16)
project(OpenGL)
set(CMAKE_CXX_STANDARD 20)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(GLEW REQUIRED)

add_subdirectory(Dependencies/glfw-3.3.2)

add_executable(OpenGL Source/main.cpp)
target_link_libraries(OpenGL PRIVATE GLEW::GLEW glfw)
