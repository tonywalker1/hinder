################################################################################
# find dependencies
################################################################################
find_package(fmt REQUIRED)

################################################################################
# build subproject
################################################################################
add_library(assert SHARED
    assert_config.cpp
)

set_target_properties(assert PROPERTIES
    OUTPUT_NAME "hinder_assert"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    CXX_STANDARD ${CMAKE_CXX_STANDARD}
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

add_library(hinder::assert ALIAS assert)

target_compile_options(assert PRIVATE ${HINDER_WARNING_FLAGS})

target_include_directories(assert
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(assert
    PUBLIC fmt::fmt
    PUBLIC hinder::core
)

################################################################################
# install library and headers
################################################################################
install(TARGETS assert
    EXPORT hinder_assertTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/hinder/assert
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/hinder
)

################################################################################
# install CMake package config files
################################################################################
set(HINDER_ASSERT_CMAKE_CONFIG_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/hinder_assert")

install(EXPORT hinder_assertTargets
    FILE hinder_assertTargets.cmake
    NAMESPACE hinder::
    DESTINATION ${HINDER_ASSERT_CMAKE_CONFIG_DIR}
)

# Generate package config file
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/hinder_assertConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/hinder_assertConfig.cmake"
    INSTALL_DESTINATION ${HINDER_ASSERT_CMAKE_CONFIG_DIR}
)

# Generate package version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/hinder_assertConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/hinder_assertConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/hinder_assertConfigVersion.cmake"
    DESTINATION ${HINDER_ASSERT_CMAKE_CONFIG_DIR}
)
