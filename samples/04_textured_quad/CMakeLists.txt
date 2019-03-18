cmake_minimum_required(VERSION 3.13)
project(textured_quad)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

include_directories("../../arc/src")
include_directories("../../libs/glad/include")
include_directories("../../libs/glfw/include")


#Bring the headers, such as Student.h into the project
include_directories(src)

#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_executable(textured_quad ${SOURCES})

target_link_libraries(textured_quad opengl32 glad glfw arc)
