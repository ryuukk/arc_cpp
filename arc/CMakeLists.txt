cmake_minimum_required(VERSION 3.13)
project(arc)

include_directories("../../arc/src")
include_directories("../../libs/glad/include/")
include_directories("../../libs/glfw/include/")

#Bring the headers, such as Student.h into the project
include_directories(include)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*")

#Generate the shared library from the sources
add_library(arc STATIC ${SOURCES})

target_link_libraries(arc opengl32 glad glfw)

#Set the location for library installation -- i.e., /usr/lib in this case
# not really necessary in this example. Use "sudo make install" to apply
# install(TARGETS arc DESTINATION /usr/lib)