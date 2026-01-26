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

# add file and line to exception messages
option(HINDER_WITH_EXCEPTION_SOURCE "Add '__FILE__ and __LINE__' to exception messages. [default = ON]" ON)
if (HINDER_WITH_EXCEPTION_SOURCE)
    add_compile_definitions(HINDER_WITH_EXCEPTION_SOURCE)
    message(STATUS "Enabling code source (i.e., __FILE__ and __LINE__) in exception messages.")
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
    message(STATUS "CMAKE_CXX_STANDARD not set. Using C++ 20.")
    set(CMAKE_CXX_STANDARD 20)
endif ()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#set(CMAKE_VERBOSE_MAKEFILE ON)

include(GNUInstallDirs)

################################################################################
# display a few import an settings for debugging build problems
################################################################################
message(STATUS "************************************************************************************")
message(STATUS "    C++ Compiler: " ${CMAKE_CXX_COMPILER})
message(STATUS "       C++ Flags: " ${CMAKE_CXX_FLAGS})
message(STATUS "    C++ Standard: " ${CMAKE_CXX_STANDARD})
message(STATUS "      Build Type: " ${CMAKE_BUILD_TYPE})
message(STATUS "    Verbose Make: " ${CMAKE_VERBOSE_MAKEFILE})
message(STATUS "     Source Path: " ${PROJECT_SOURCE_DIR})
message(STATUS "      Build Path: " ${PROJECT_BINARY_DIR})
message(STATUS "************************************************************************************")
message(STATUS "     Build Tests: " ${HINDER_WITH_TESTS})
message(STATUS "Exception Source: " ${HINDER_WITH_EXCEPTION_SOURCE})
message(STATUS "Exception Format: " ${HINDER_EXCEPTION_MESSAGE_FORMAT})
message(STATUS "   Assert Format: " ${HINDER_ASSERT_MESSAGE_FORMAT})
message(STATUS "************************************************************************************")

################################################################################
# find dependencies
################################################################################

################################################################################
# build subprojects
################################################################################
add_subdirectory(src/assert)
add_subdirectory(src/core)
add_subdirectory(src/exception)
add_subdirectory(src/misc)

if (HINDER_WITH_TESTS)
    message(STATUS "Testing is enabled. Tests will be built.")
    enable_testing()
    add_subdirectory(tests/assert)
#    add_subdirectory(tests/core)
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
