cmake_minimum_required(VERSION 3.13)
project(hello)


include_directories("../../arc/src")
include_directories("../../libs/glad/src")


#Bring the headers, such as Student.h into the project
include_directories(src)

#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_executable(hello ${SOURCES})

target_link_libraries(hello glad arc)
