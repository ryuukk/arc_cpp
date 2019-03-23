cmake_minimum_required(VERSION 3.13)
project(arc_cpp)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug) # Release, RelWithDebInfo, Debug


option (ARC_SAMPLES "Enable arc samples" ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/glad)
add_subdirectory(libs/glfw)
add_subdirectory(arc)


if (ARC_SAMPLES)
    add_subdirectory(samples)
endif ()