cmake_minimum_required(VERSION 3.5)
project(PSGenerator)

set(CMAKE_CXX_STANDARD 11)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-D_DEBUG)
    set(CONFIG_DIR "debug" CACHE STRING "Build config directory" FORCE)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_definitions(-DNDEBUG)
    set(CONFIG_DIR "release" CACHE STRING "Build config directory" FORCE)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
    add_definitions(-DNDEBUG)
    set(CONFIG_DIR "releaseMinSize" CACHE STRING "Build config directory" FORCE)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    add_definitions(-DNDEBUG)
    set(CONFIG_DIR "releaseMinSize" CACHE STRING "Build config directory" FORCE)
else()
    message(FATAL_ERROR "Invalid build type: " ${CMAKE_BUILD_TYPE})
endif()

set(OUTPUT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/output)
message(STATUS "Output base directory: " ${OUTPUT_BASE_DIR})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

add_subdirectory(libclang)

include(setup_target_properties_executable)
include(show_target_properties)
include(display_list)

message("Setting up ${PROJECT_NAME}")

set(PACKAGE_NAME ${PROJECT_NAME})
set(TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "Proxy/Stub generator")
set(PACKAGE_VERSION_MAJOR 1)
set(PACKAGE_VERSION_MINOR 0)
set(PACKAGE_VERSION_MICRO 0)
set(PACKAGE_VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})

set(PACKAGE_DEFINITIONS
    ${COMPILER_DEFINITIONS_CXX}
    PSGENERATOR)

set(PACKAGE_INCLUDE_DIRS
    .
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
    ${PACKAGE_DEPENDENCIES})

set(PACKAGE_SOURCES main.cpp)
file(GLOB PARSER_INCLUDES include/*.h)
file(GLOB PARSER_SOURCES src/*.cpp)

list(APPEND PACKAGE_SOURCES ${PARSER_SOURCES})

set(PACKAGE_INCLUDES )
list(APPEND PACKAGE_INCLUDES ${PARSER_INCLUDES})

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

setup_target_properties_executable(${PROJECT_NAME})

if (CMAKE_VERBOSE_MAKEFILE)
    show_target_properties(${PROJECT_NAME})
endif()

## uninstall target
#configure_file(
#    ${WPEFRAMEWORK_PACKAGES_ROOT}/cmake/uninstall.cmake.in
#    ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake
#    IMMEDIATE @ONLY)
#
#add_custom_target(uninstall_${PROJECT_NAME}
#    COMMAND ${CMAKE_COMMAND} -DCOMPONENT=${PROJECT_NAME} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake
#    DEPENDS ${PROJECT_NAME}
#    COMMENT "Uninstalling ${PROJECT_NAME}")
#
#add_custom_target(install_${PROJECT_NAME}
#    COMMAND ${CMAKE_COMMAND} -DCOMPONENT=${PROJECT_NAME} -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
#    DEPENDS ${PROJECT_NAME}
#    COMMENT "Installing ${PROJECT_NAME}")
#
#add_dependencies(install-component-${COMPONENT_NAME} install_${PROJECT_NAME})
#
#add_dependencies(uninstall-component-${COMPONENT_NAME} uninstall_${PROJECT_NAME})
#
#install(
#    TARGETS ${PROJECT_NAME} DESTINATION
#    ${CMAKE_INSTALL_PREFIX}/lib/wpeframework/plugins
#    COMPONENT ${PROJECT_NAME})

add_subdirectory(test)
