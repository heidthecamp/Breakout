CMAKE_MINIMUM_REQUIRED(VERSION 3.21 FATAL_ERROR)

# Determine if this file has been included by ADD_SUBDIRECTORY or run directly from cmake
# If the file was included, it will then run a sub-cmake in order to configure and build
# the required dependencies during the main project's configure step.
IF(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)

    SET(_source_dir ${CMAKE_SOURCE_DIR}/ThirdParty)
    SET(_binary_dir ${CMAKE_BINARY_DIR}/ThirdParty)

    # Set *_ROOT variables for config-based packages
    SET(SFML_ROOT ${_binary_dir}/SFML PARENT_SCOPE)

    # Set CMAKE_PREFIX_PATH for find-based packages
    LIST(APPEND CMAKE_PREFIX_PATH
    )
    SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} PARENT_SCOPE)

    ###
    ### Configure
    ###

    FILE(MAKE_DIRECTORY ${_source_dir})

    EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" 
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            ${_source_dir}
        WORKING_DIRECTORY ${_binary_dir}
        RESULT_VARIABLE _result
    )
    IF(_result)
        MESSAGE(SEND_ERROR "${_result}")
        MESSAGE(FATAL_ERROR "Failed to configure ThirdParty projects")
    ENDIF()

    ###
    ### Build
    ###

    EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${_binary_dir}
        RESULT_VARIABLE _result
    )
    IF(_result)
        MESSAGE(SEND_ERROR "${_result}")
        MESSAGE(FATAL_ERROR "Failed to build ThirdParty projects")
    ENDIF()

ELSE()

    LIST(PREPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../CMake")
    
    INCLUDE(ExternalProject)

    PROJECT(ThirdParty)

    ###
    ### SFML
    ###
    
    SET(SFML_ROOT       ${CMAKE_BINARY_DIR}/SFML)
    SET(SFML_SOURCE_DIR ${CMAKE_BINARY_DIR}/SFML_src)
    SET(SFML_BINARY_DIR ${CMAKE_BINARY_DIR}/SFML_bin)

    FIND_PACKAGE(SFML COMPONENTS graphics audio CONFIG QUIET)

    IF(NOT SFML_FOUND)
        ExternalProject_Add(
            ThirdParty_SFML
            URL                 "https://www.sfml-dev.org/files/SFML-2.6.0-sources.zip"
            URL_HASH            MD5=b504f958ea5070b183626fe8b3414db5
            SOURCE_DIR          ${SFML_SOURCE_DIR}
            BINARY_DIR          ${SFML_BINARY_DIR}
            CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${SFML_ROOT}
                                -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            BUILD_COMMAND       ""
            UPDATE_COMMAND      ""
            TEST_COMMAND        ""
            INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install
        )
    ENDIF()

ENDIF()


