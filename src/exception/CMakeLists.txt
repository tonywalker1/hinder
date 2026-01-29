################################################################################
# build subproject
################################################################################
add_library(exception SHARED
    exception_config.cpp
    string.cpp
)

set_target_properties(exception PROPERTIES
    OUTPUT_NAME "hinder_exception"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    CXX_STANDARD ${CMAKE_CXX_STANDARD}
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

add_library(hinder::exception ALIAS exception)

target_compile_options(exception PRIVATE ${HINDER_WARNING_FLAGS})

target_include_directories(exception
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(exception
    PUBLIC hinder::core
)

################################################################################
# install library and headers
################################################################################
install(TARGETS exception
    EXPORT hinder_exceptionTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/hinder/exception
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/hinder
)

################################################################################
# install CMake package config files
################################################################################
set(HINDER_EXCEPTION_CMAKE_CONFIG_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/hinder_exception")

install(EXPORT hinder_exceptionTargets
    FILE hinder_exceptionTargets.cmake
    NAMESPACE hinder::
    DESTINATION ${HINDER_EXCEPTION_CMAKE_CONFIG_DIR}
)

# Generate package config file
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/hinder_exceptionConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/hinder_exceptionConfig.cmake"
    INSTALL_DESTINATION ${HINDER_EXCEPTION_CMAKE_CONFIG_DIR}
)

# Generate package version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/hinder_exceptionConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/hinder_exceptionConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/hinder_exceptionConfigVersion.cmake"
    DESTINATION ${HINDER_EXCEPTION_CMAKE_CONFIG_DIR}
)
