cmake_minimum_required(VERSION 3.13)
project(arc)

include_directories("../../arc/src")
include_directories("../../libs/glad/include/")
include_directories("../../libs/glfw/include/")

include_directories(include)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.c src/*.h)

add_library(arc STATIC ${SOURCES} src/gfx/VertexBuffer.cpp src/gfx/VertexBuffer.h)

if(WIN32)
    target_link_libraries(arc opengl32 glad glfw)
else()
    target_link_libraries(arc glad glfw spdlog)
endif()

# install(TARGETS arc DESTINATION /usr/lib)