################################################################################
# find dependencies
################################################################################
find_package(fmt REQUIRED)

################################################################################
# collect all source files
################################################################################
set(HINDER_SOURCES
    # core
    core/timestamp.cpp
    # exception
    exception/exception.cpp
    exception/format.cpp
)

################################################################################
# build library
################################################################################
add_library(hinder SHARED ${HINDER_SOURCES})

set_target_properties(hinder PROPERTIES
    OUTPUT_NAME "hinder"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    CXX_STANDARD ${CMAKE_CXX_STANDARD}
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

add_library(hinder::hinder ALIAS hinder)

target_compile_options(hinder PRIVATE ${HINDER_WARNING_FLAGS})

target_include_directories(hinder
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(hinder
    PUBLIC fmt::fmt
)

################################################################################
# install library and headers
################################################################################
install(TARGETS hinder
    EXPORT hinderTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install all headers
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/hinder
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

################################################################################
# install CMake package config files
################################################################################
set(HINDER_CMAKE_CONFIG_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/hinder")

install(EXPORT hinderTargets
    FILE hinderTargets.cmake
    NAMESPACE hinder::
    DESTINATION ${HINDER_CMAKE_CONFIG_DIR}
)

# Generate package config file
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/hinderConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/hinderConfig.cmake"
    INSTALL_DESTINATION ${HINDER_CMAKE_CONFIG_DIR}
)

# Generate package version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/hinderConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/hinderConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/hinderConfigVersion.cmake"
    DESTINATION ${HINDER_CMAKE_CONFIG_DIR}
)
