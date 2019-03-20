set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

function(add_sample sample_name)
    project(${sample_name})

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${sample_name}/../../arc/src)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${sample_name}/../../libs/glad/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${sample_name}/../../libs/glfw/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${sample_name}/src)

    file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${sample_name}/src/*.cpp")

    add_executable(${PROJECT_NAME} ${SOURCES})

    if(WIN32)
        # set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
    endif()

    target_link_libraries(${PROJECT_NAME} opengl32 glad glfw arc)
endfunction()

add_sample(01_hello)
add_sample(02_input)
add_sample(03_triangle)
add_sample(04_textured_quad)
add_sample(05_spritebatch)
add_sample(06_cube)
add_sample(07_model)
add_sample(08_renderbatch)
add_sample(09_skeletal_animation)
add_sample(99_experiments)