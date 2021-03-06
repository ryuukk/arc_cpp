set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/../bin)
set(VS_DEBUGGER_WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/../bin)

function(add_sample sample_name)
    project(${sample_name})

    set(EXE_NAME "${PROJECT_NAME}_${CMAKE_BUILD_TYPE}")
    string(TOLOWER ${EXE_NAME} EXE_NAME)


    # set (CMAKE_CXX_FLAGS "-Wall -Wextra")
    # set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -m64 -Ofast -flto")
    # set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--allow-multiple-definition")

    if (WIN32)
        if (MSVC)
        else ()
            set(CMAKE_CXX_FLAGS_DEBUG "-march=native -g -m64 -O0 -flto")
            set(CMAKE_CXX_FLAGS_RELEASE "-march=native -m64 -03 -Wl,-subsystem,windows")
        endif ()
    else ()

        set(CMAKE_CXX_FLAGS_DEBUG "-march=native -g -m64 -O0 -flto")
        set(CMAKE_CXX_FLAGS_RELEASE "-march=native -m64 -0fast -flto")
    endif ()

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${sample_name}/../../arc/src)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${sample_name}/../../libs/glad/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${sample_name}/../../libs/glfw/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${sample_name}/src)

    file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${sample_name}/src/*.cpp")

    add_executable(${PROJECT_NAME} ${SOURCES})

    if (MSVC)
        set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /debug:fastlink")
        set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO} /debug:fastlink")
		set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/../bin)
		set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${EXE_NAME}")
	else ()
		set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${EXE_NAME}")
	endif()


    if (WIN32)
        # target_link_libraries(${PROJECT_NAME} -mwindows opengl32 glad glfw arc)
        target_link_libraries(${PROJECT_NAME} opengl32 glad glfw arc ws2_32)
    else ()
        target_link_libraries(${PROJECT_NAME} glad glfw arc)
    endif ()

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
add_sample(10_bitmap_font)
add_sample(11_texture_atlas)
add_sample(12_shapes)
add_sample(13_gui)
add_sample(99_experiments)