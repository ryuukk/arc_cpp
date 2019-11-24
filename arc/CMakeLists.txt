cmake_minimum_required(VERSION 3.13)
project(arc)

include_directories("../../arc/src")
include_directories("../../libs/glad/include/")
include_directories("../../libs/glfw/include/")

include_directories(include)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.c src/*.h)

add_library(arc STATIC ${SOURCES} ../samples/99_experiments/src/io.h ../samples/99_experiments/src/packets.h ../samples/99_experiments/src/client.h src/utils/StopWatch.h)

if(WIN32)
    target_link_libraries(arc opengl32 glad glfw ws2_32)
else()
    target_link_libraries(arc glad glfw)
endif()

# install(TARGETS arc DESTINATION /usr/lib)