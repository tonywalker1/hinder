################################################################################
# project settings
################################################################################
cmake_minimum_required(VERSION 3.20)
project(hinder
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "C++ utility libraries for compile-time safety and modern practices"
)

################################################################################
# project options
################################################################################

# global options
option(HINDER_WITH_TESTS "Build tests. [default = ON]" ON)

# add file and line to exception messages
option(HINDER_WITH_EXCEPTION_SOURCE "Add '__FILE__ and __LINE__' to exception messages. [default = ON]" ON)
if (HINDER_WITH_EXCEPTION_SOURCE)
    add_compile_definitions(HINDER_WITH_EXCEPTION_SOURCE)
    message(STATUS "Enabling code source (i.e., __FILE__ and __LINE__) in exception messages.")
endif ()

################################################################################
# global build settings
################################################################################
# Set C++ standard as project default (targets can override if needed)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard version")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default to Release build if not specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "CMAKE_BUILD_TYPE not set. Using Release.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

# Common warning flags to be applied to targets
set(HINDER_WARNING_FLAGS -Wall -Wextra -Wpedantic)

#set(CMAKE_VERBOSE_MAKEFILE ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

################################################################################
# display build configuration for debugging
################################################################################
message(STATUS "************************************************************************************")
message(STATUS "  Project: ${PROJECT_NAME} ${PROJECT_VERSION}")
message(STATUS "  C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Warning Flags: ${HINDER_WARNING_FLAGS}")
message(STATUS "  Source Path: ${PROJECT_SOURCE_DIR}")
message(STATUS "  Build Path: ${PROJECT_BINARY_DIR}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "************************************************************************************")
message(STATUS "  Build Tests: ${HINDER_WITH_TESTS}")
message(STATUS "  Exception Source: ${HINDER_WITH_EXCEPTION_SOURCE}")
message(STATUS "************************************************************************************")

################################################################################
# find dependencies
################################################################################

################################################################################
# build library
################################################################################
add_subdirectory(src)

if (HINDER_WITH_TESTS)
    message(STATUS "Testing is enabled. Tests will be built.")
    enable_testing()
    add_subdirectory(tests/core)
    add_subdirectory(tests/exception)
    add_subdirectory(tests/misc)
endif ()

