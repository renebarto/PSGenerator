cmake_minimum_required(VERSION 3.8)
project(PSGenerator.test)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 11)

include(setup_target_properties_library)
include(show_target_properties)
include(display_list)

message("Setting up ${PROJECT_NAME}")

add_subdirectory(googletest)

set(PACKAGE_NAME ${PROJECT_NAME})
set(TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "Proxy/Stub generator tests")
set(PACKAGE_VERSION_MAJOR 1)
set(PACKAGE_VERSION_MINOR 0)
set(PACKAGE_VERSION_MICRO 0)
set(PACKAGE_VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})

set(PACKAGE_DEFINITIONS
    ${COMPILER_DEFINITIONS_CXX}
    TESTDATA_ROOT="${CMAKE_SOURCE_DIR}/testdata")

set(PACKAGE_INCLUDE_DIRS
    .
    ./googletest
    ..
    ${LIB_CLANG_INCLUDE_DIRS})

set(PACKAGE_OPTIONS
    ${COMPILER_OPTIONS_CXX})

set(PACKAGE_LINK_OPTIONS)

set(PACKAGE_DEPENDENCIES
    )

set(PACKAGE_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    ${LIB_CLANG_LIB}
    gtest
    ${PACKAGE_DEPENDENCIES})

set(PACKAGE_SOURCES gtest_main.cc)
file(GLOB TEST_INCLUDES include/*.h)
file(GLOB TEST_SOURCES src/*.cpp)
file(GLOB PARSER_INCLUDES ../include/*.h)
file(GLOB PARSER_SOURCES ../src/*.cpp)

list(APPEND PACKAGE_SOURCES ${TEST_SOURCES} ${PARSER_SOURCES})

set(PACKAGE_INCLUDES )
list(APPEND PACKAGE_INCLUDES ${TEST_INCLUDES} ${PARSER_INCLUDES})

set(PACKAGE_INPUT
    ${PACKAGE_SOURCES}
    ${PACKAGE_INCLUDES}
    )

if (BUILD_REFERENCE)
    list(APPEND PACKAGE_DEFINITIONS BUILD_REFERENCE=${BUILD_REFERENCE})
endif()

if (CMAKE_VERBOSE_MAKEFILE)
    display_list("Defines                     : " ${PACKAGE_DEFINITIONS} )
    display_list("Compiler options            : " ${PACKAGE_OPTIONS} )
    display_list("Source files                : " ${PACKAGE_SOURCES} )
    display_list("Include files               : " ${PACKAGE_INCLUDES} )
    display_list("Include dirs                : " ${PACKAGE_INCLUDE_DIRS} )
    display_list("Link libs                   : " ${PACKAGE_LIBS} )
    display_list("Linker options              : " ${PACKAGE_LINK_OPTIONS} )
    display_list("Dependencies                : " ${PACKAGE_DEPENDENCIES} )
endif()

add_executable(${PROJECT_NAME} ${PACKAGE_INPUT})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${PACKAGE_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PRIVATE ${PACKAGE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${PACKAGE_LIBS})
list_to_string(PACKAGE_LINK_OPTIONS PACKAGE_LINK_OPTIONS_STRING)
if (NOT "${PACKAGE_LINK_OPTIONS_STRING}" STREQUAL "")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${PACKAGE_LINK_OPTIONS_STRING}")
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PACKAGE_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

setup_target_properties_library(${PROJECT_NAME})

if (CMAKE_VERBOSE_MAKEFILE)
    show_target_properties(${PROJECT_NAME})
endif()

