cmake_minimum_required (VERSION 3.18)
project(Game_of_life)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)


if(MSVC)
    if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
        set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
    else()
        set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin32/glslangValidator.exe")
    endif()
elseif(UNIX)
    set(GLSL_VALIDATOR "glslangValidator")
endif()


file(GLOB_RECURSE DATA_FILES
            "data/*.json"
            "data/*.png"
            "data/*.jpg"
            "data/*.jpeg"
            "data/*.bmp"
            "data/*.hdr"
            "data/*.obj"
            "data/*.mtl"
            "data/*.gltf"
            "data/*.bin"
            )
    foreach(DATA ${DATA_FILES})
        get_filename_component(FILE_NAME ${DATA} NAME)
        get_filename_component(PATH_NAME ${DATA} DIRECTORY)
        get_filename_component(EXTENSION ${DATA} EXT)
        file(RELATIVE_PATH PATH_NAME "${CMAKE_CURRENT_SOURCE_DIR}" ${PATH_NAME})
        #MESSAGE("Data PATH: ${PATH_NAME} NAME: ${FILE_NAME}")
        set(DATA_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${PATH_NAME}/${FILE_NAME}")
        #MESSAGE("Data OUT PATH: ${DATA_OUTPUT}")
        add_custom_command(
                OUTPUT ${DATA_OUTPUT}
                COMMAND ${CMAKE_COMMAND} -E copy
                ${DATA}
                ${DATA_OUTPUT}
                DEPENDS ${DATA})
        list(APPEND Data_OUTPUT_FILES ${DATA_OUTPUT})
    endforeach(DATA)


    add_custom_target(
            data_target
            DEPENDS ${Data_OUTPUT_FILES}
    )


file(GLOB_RECURSE COMMON_FILES src/*.cpp include/*.h)
add_library(Common STATIC ${COMMON_FILES})
target_include_directories(Common PUBLIC include/)
target_link_libraries(Common PUBLIC SDL2::SDL2 SDL2::SDL2main imgui::imgui)
set_target_properties(Common PROPERTIES UNITY_BUILD ON)
add_dependencies(Common data_target)

if(MSVC)
    target_compile_definitions(Common PUBLIC "_USE_MATH_DEFINES" WIN32_LEAN_AND_MEAN)
    target_compile_options(Common PUBLIC /arch:AVX2 /Oi /GL /fp:fast)
    target_link_options(Common PUBLIC /LTCG)
else()

endif()

file(GLOB MAIN_FILES main/*.cpp)
foreach(MAIN_FILE ${MAIN_FILES})
    get_filename_component(MAIN_NAME ${MAIN_FILE} NAME_WE)
    
    add_executable(${MAIN_NAME} ${MAIN_FILE})
    target_link_libraries(${MAIN_NAME} PUBLIC Common) 
endforeach()