################################################################################
# project settings
################################################################################
cmake_minimum_required(VERSION 3.13.4)
project(hinder LANGUAGES CXX)

option(HINDER_WITH_TESTS "Build tests when ON (default = ON)" ON)

################################################################################
# global build settings
################################################################################
if (NOT CMAKE_CXX_FLAGS)
    message(STATUS "CMAKE_CXX_FLAGS not set. Using defaults.")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
endif ()

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE not set. Using release.")
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (NOT CMAKE_CXX_STANDARD)
    message(STATUS "CMAKE_CXX_STANDARD not set. Using C++ 17.")
    set(CMAKE_CXX_STANDARD 17)
endif ()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

################################################################################
# display a few import an settings for debugging build problems
################################################################################
message(STATUS "************")
message(STATUS "C++ Compiler: " ${CMAKE_CXX_COMPILER})
message(STATUS "   C++ Flags: " ${CMAKE_CXX_FLAGS})
message(STATUS "C++ Standard: " ${CMAKE_CXX_STANDARD})
message(STATUS "  Build Type: " ${CMAKE_BUILD_TYPE})
message(STATUS "Verbose Make: " ${CMAKE_VERBOSE_MAKEFILE})
message(STATUS " Source Path: " ${PROJECT_SOURCE_DIR})
message(STATUS "  Build Path: " ${PROJECT_BINARY_DIR})
message(STATUS "************")

################################################################################
# find dependencies
################################################################################

################################################################################
# build subprojects
################################################################################
add_subdirectory(src/misc)

if (HINDER_WITH_TESTS)
    message(STATUS "Testing is enabled. Tests will be built.")
    enable_testing()
    add_subdirectory(tests/misc)
endif ()

################################################################################
# install library and subprojects
################################################################################
install(
    FILES "${PROJECT_SOURCE_DIR}/cmake/hinderConfig.cmake"
    DESTINATION "lib/cmake/hinder"
)

