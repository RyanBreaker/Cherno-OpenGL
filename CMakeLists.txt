cmake_minimum_required(VERSION 3.16)
project(OpenGL)
set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

add_executable(OpenGL Source/main.cpp)
target_link_libraries(OpenGL PRIVATE glfw GLEW::GLEW)
