cmake_minimum_required(VERSION 3.13)
project(arc_cpp)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libs/glad)
add_subdirectory(libs/glfw)
add_subdirectory(arc)
add_subdirectory(samples/01_hello)