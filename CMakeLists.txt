################################################################################
# project settings
################################################################################
cmake_minimum_required(VERSION 3.13.4)
project(hinder LANGUAGES CXX)

################################################################################
# project options
################################################################################

# global options
option(HINDER_WITH_TESTS "Build tests. [default = ON]" ON)

#
# exception options
#

# add file and line to messages
option(HINDER_WITH_EXCEPTION_SOURCE "Add '__FILE__ and __LINE__' to exception messages. [default = ON]" ON)
if (HINDER_WITH_EXCEPTION_SOURCE)
    add_compile_definitions(HINDER_WITH_EXCEPTION_SOURCE)
    message(STATUS "Enabling code source (i.e., __FILE__ and __LINE__) in exception messages.")
endif ()

# format of exception messages
set(HINDER_EXCEPTION_MESSAGE_FORMAT Default CACHE STRING "Format of all exception messages (Default|User|Structured). [default = Default]")
set_property(CACHE HINDER_EXCEPTION_MESSAGE_FORMAT PROPERTY STRINGS Default User Structured)
if (HINDER_EXCEPTION_MESSAGE_FORMAT STREQUAL "Default")
    message(STATUS "Enabling default messages for all exceptions messages.")
    add_compile_definitions(HINDER_DEFAULT_EXCEPTION_MESSAGE)
elseif (HINDER_EXCEPTION_MESSAGE_FORMAT STREQUAL "Structured")
    message(STATUS "Enabling structured messages for all exception messages.")
    add_compile_definitions(HINDER_STRUCTURED_EXCEPTION_MESSAGE)
elseif (HINDER_EXCEPTION_MESSAGE_FORMAT STREQUAL "User")
    message(STATUS "Enabling user defined messages for all exception messages.")
    add_compile_definitions(HINDER_USER_EXCEPTION_MESSAGE)
else ()
    message(WARNING "Unknown setting for HINDER_EXCEPTION_MESSAGE_FORMAT. Using Default instead.")
    message(STATUS "Enabling default messages for all exceptions.")
    add_compile_definitions(HINDER_DEFAULT_EXCEPTION_MESSAGE)
endif ()

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

#set(CMAKE_VERBOSE_MAKEFILE ON)

include(GNUInstallDirs)

################################################################################
# display a few import an settings for debugging build problems
################################################################################
message(STATUS "************")
message(STATUS "    C++ Compiler: " ${CMAKE_CXX_COMPILER})
message(STATUS "       C++ Flags: " ${CMAKE_CXX_FLAGS})
message(STATUS "    C++ Standard: " ${CMAKE_CXX_STANDARD})
message(STATUS "      Build Type: " ${CMAKE_BUILD_TYPE})
message(STATUS "    Verbose Make: " ${CMAKE_VERBOSE_MAKEFILE})
message(STATUS "     Source Path: " ${PROJECT_SOURCE_DIR})
message(STATUS "      Build Path: " ${PROJECT_BINARY_DIR})
message(STATUS "************")
message(STATUS "     Build Tests: " ${HINDER_WITH_TESTS})
message(STATUS "Exception Source: " ${HINDER_WITH_EXCEPTION_SOURCE})
message(STATUS "Exception Format: " ${HINDER_EXCEPTION_MESSAGE_FORMAT})
message(STATUS "************")

################################################################################
# find dependencies
################################################################################

################################################################################
# build subprojects
################################################################################
add_subdirectory(src/exception)
add_subdirectory(src/misc)

if (HINDER_WITH_TESTS)
    message(STATUS "Testing is enabled. Tests will be built.")
    enable_testing()
    add_subdirectory(tests/exception)
    add_subdirectory(tests/misc)
endif ()

################################################################################
# install library and subprojects
################################################################################
install(
    FILES "${PROJECT_SOURCE_DIR}/cmake/hinderConfig.cmake"
    DESTINATION "lib/cmake/hinder"
)
