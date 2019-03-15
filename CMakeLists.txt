cmake_minimum_required(VERSION 3.13)
project(arc_cpp)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug) # Release, RelWithDebInfo, Debug

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/glad)
add_subdirectory(libs/glfw)
add_subdirectory(arc)

add_subdirectory(samples/01_hello)
add_subdirectory(samples/02_input)
add_subdirectory(samples/03_triangle)
add_subdirectory(samples/06_cube)
add_subdirectory(samples/07_model)