# - Try to find unittest-c++
# Once done this will define
#  UNITTEST_CPP_FOUND - System has unittest-c++
#  UNITTEST_CPP_INCLUDE_DIRS - The unittest-c++ include directories
#  UNITTEST_CPP_LIBRARIES - The libraries needed to use unittest-c++
#

find_package(PkgConfig)
pkg_check_modules(PC_UNITTEST_CPP unittest-c++)

if(PC_UNITTEST_CPP_FOUND)
    if(UNITTEST_CPP_FIND_VERSION AND PC_UNITTEST_CPP_VERSION)
        if ("${UNITTEST_CPP_FIND_VERSION}" VERSION_GREATER "${PC_UNITTEST_CPP_VERSION}")
            message(WARNING "Incorrect version, found ${PC_UNITTEST_CPP_VERSION}, need at least ${UNITTEST_CPP_FIND_VERSION}, please install correct version ${UNITTEST_CPP_FIND_VERSION}")
            set(UNITTEST_CPP_FOUND_TEXT "Found incorrect version")
            unset(PC_UNITTEST_CPP_FOUND)
        endif()
    endif()
else()
    set(UNITTEST_CPP_FOUND_TEXT "Not found")
endif()

if(PC_UNITTEST_CPP_FOUND)
    find_path(
        UNITTEST_CPP_INCLUDE_DIRS
        NAMES unittest-c++/UnitTestC++.h
        HINTS ${PC_UNITTEST_CPP_INCLUDEDIR} ${PC_UNITTEST_CPP_INCLUDE_DIRS})

    set(UNITTEST_CPP_LIBS unittest-c++)
    set(UNITTEST_CPP_LIBRARY )
    foreach(LIB ${UNITTEST_CPP_LIBS})
        find_library(UNITTEST_CPP_LIBRARY_${LIB} NAMES ${LIB}
            HINTS ${PC_UNITTEST_CPP_LIBDIR} ${PC_UNITTEST_CPP_LIBRARY_DIRS})
        list(APPEND UNITTEST_CPP_LIBRARY ${UNITTEST_CPP_LIBRARY_${LIB}})
    endforeach()

    if("${UNITTEST_CPP_INCLUDE_DIRS}" STREQUAL "" OR "${UNITTEST_CPP_LIBRARY}" STREQUAL "")
        set(UNITTEST_CPP_FOUND_TEXT "Not found")
    else()
        set(UNITTEST_CPP_FOUND_TEXT "Found")
    endif()
else()
    set(UNITTEST_CPP_FOUND_TEXT "Not found")
endif()

if (CMAKE_VERBOSE_MAKEFILE)
    message(STATUS "unittest-c++   : ${UNITTEST_CPP_FOUND_TEXT}")
    message(STATUS "  version      : ${PC_UNITTEST_CPP_VERSION}")
    message(STATUS "  cflags       : ${PC_UNITTEST_CPP_CFLAGS}")
    message(STATUS "  cflags other : ${PC_UNITTEST_CPP_CFLAGS_OTHER}")
    message(STATUS "  include dirs : ${PC_UNITTEST_CPP_INCLUDE_DIRS} ${PC_UNITTEST_CPP_INCLUDEDIR}")
    message(STATUS "  lib dirs     : ${PC_UNITTEST_CPP_LIBRARY_DIRS} ${PC_UNITTEST_CPP_LIBDIR}")
    message(STATUS "  include dirs : ${UNITTEST_CPP_INCLUDE_DIRS}")
    message(STATUS "  libs         : ${UNITTEST_CPP_LIBRARY}")
endif()

set(UNITTEST_CPP_DEFINITIONS ${PC_UNITTEST_CPP_PLUGINS_CFLAGS_OTHER})
set(UNITTEST_CPP_INCLUDE_DIR ${UNITTEST_CPP_INCLUDE_DIRS})
set(UNITTEST_CPP_LIBRARIES ${UNITTEST_CPP_LIBRARY})
set(UNITTEST_CPP_LIBRARY_DIRS ${PC_UNITTEST_CPP_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(UNITTEST_CPP DEFAULT_MSG
    UNITTEST_CPP_LIBRARIES
    UNITTEST_CPP_INCLUDE_DIRS)

if(UNITTEST_CPP_FOUND)
else()
    message(WARNING "Could not find unittest-c++")
endif()

mark_as_advanced(
    UNITTEST_CPP_DEFINITIONS
    UNITTEST_CPP_INCLUDE_DIRS
    UNITTEST_CPP_LIBRARIES
    UNITTEST_CPP_LIBRARY_unittest-c++)