cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

# Allow for custom CMake modules
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

project(Breakout)

###
### Global Configuration
###

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

###
### Third Party Dependencies
###

add_subdirectory(ThirdParty)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

find_package(SFML COMPONENTS graphics audio CONFIG REQUIRED)

###
### Globals
###

include(GetRuntimeDir)

get_runtime_dir(sfml-graphics _SFML_graphics_runtime_dir)
get_runtime_dir(sfml-audio _SFML_audio_runtime_dir)

set(_RUNTIME_PATH_LIST
    ${_SFML_graphics_runtime_dir}
    ${_SFML_audio_runtime_dir}
    CACHE INTERNAL ""
)

list(REMOVE_DUPLICATES _RUNTIME_PATH_LIST)

###
### Targets
###

file(GLOB_RECURSE
    _source_list
    Source/*.hpp
    Source/*.cpp
)

add_executable(
    Breakout
    ${_source_list}
)

target_link_libraries(
    Breakout
    PRIVATE
        sfml-graphics
        sfml-audio
)

add_custom_command(
    TARGET Breakout POST_BUILD
    COMMENT "Adding Launch Target"
    COMMAND ${Python3_EXECUTABLE}
        ${CMAKE_SOURCE_DIR}/Scripts/add-launch-target.py
        --name "Breakout"
        --executable "$<TARGET_FILE:Breakout>" 
        --runtimePathList "${_RUNTIME_PATH_LIST}"
        --binaryDir "${CMAKE_BINARY_DIR}"
        --workingDir "${CMAKE_SOURCE_DIR}"
    VERBATIM
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
