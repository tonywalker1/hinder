################################################################################
# build subproject
################################################################################
add_library(core SHARED
    timestamp.cpp
)

set_target_properties(core PROPERTIES
    OUTPUT_NAME "hinder_core"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    CXX_STANDARD ${CMAKE_CXX_STANDARD}
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

add_library(hinder::core ALIAS core)

target_compile_options(core PRIVATE ${HINDER_WARNING_FLAGS})

target_include_directories(core
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

################################################################################
# install library and headers
################################################################################
install(TARGETS core
    EXPORT hinder_coreTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/hinder/core
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/hinder
)

################################################################################
# install CMake package config files
################################################################################
set(HINDER_CORE_CMAKE_CONFIG_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/hinder_core")

install(EXPORT hinder_coreTargets
    FILE hinder_coreTargets.cmake
    NAMESPACE hinder::
    DESTINATION ${HINDER_CORE_CMAKE_CONFIG_DIR}
)

# Generate package config file
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/hinder_coreConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/hinder_coreConfig.cmake"
    INSTALL_DESTINATION ${HINDER_CORE_CMAKE_CONFIG_DIR}
)

# Generate package version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/hinder_coreConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/hinder_coreConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/hinder_coreConfigVersion.cmake"
    DESTINATION ${HINDER_CORE_CMAKE_CONFIG_DIR}
)
