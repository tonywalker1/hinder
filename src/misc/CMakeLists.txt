################################################################################
# build subproject (header-only INTERFACE library)
################################################################################
add_library(misc INTERFACE)

add_library(hinder::misc ALIAS misc)

target_include_directories(misc
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

################################################################################
# install headers
################################################################################
install(TARGETS misc
    EXPORT hinder_miscTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/hinder/misc
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/hinder
)

################################################################################
# install CMake package config files
################################################################################
set(HINDER_MISC_CMAKE_CONFIG_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/hinder_misc")

install(EXPORT hinder_miscTargets
    FILE hinder_miscTargets.cmake
    NAMESPACE hinder::
    DESTINATION ${HINDER_MISC_CMAKE_CONFIG_DIR}
)

# Generate package config file
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/hinder_miscConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/hinder_miscConfig.cmake"
    INSTALL_DESTINATION ${HINDER_MISC_CMAKE_CONFIG_DIR}
)

# Generate package version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/hinder_miscConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/hinder_miscConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/hinder_miscConfigVersion.cmake"
    DESTINATION ${HINDER_MISC_CMAKE_CONFIG_DIR}
)
