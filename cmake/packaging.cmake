################################################################################
# CPack packaging configuration
# Generates RPM and DEB packages for runtime and development components.
#
# Runtime component:
#   RPM: hinder         DEB: libhinder0
#   Contents: libhinder.so.0, libhinder.so.0.1.0
#
# Devel component:
#   RPM: hinder-devel   DEB: libhinder-dev
#   Contents: headers, CMake config files
################################################################################

set(CPACK_PACKAGE_NAME "hinder")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_CONTACT "https://github.com/tonywalker1/hinder")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

set(CPACK_COMPONENTS_ALL runtime devel)
set(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME "hinder Runtime Library")
set(CPACK_COMPONENT_RUNTIME_DESCRIPTION "Shared library for hinder C++ utilities.")
set(CPACK_COMPONENT_DEVEL_DISPLAY_NAME "hinder Development Files")
set(CPACK_COMPONENT_DEVEL_DESCRIPTION "Headers and CMake config files for building against hinder.")
set(CPACK_COMPONENT_DEVEL_DEPENDS runtime)

################################################################################
# RPM settings
################################################################################
set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_RPM_PACKAGE_GROUP "System/Libraries")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_RELEASE "1")

set(CPACK_RPM_RUNTIME_PACKAGE_NAME "hinder")
set(CPACK_RPM_DEVEL_PACKAGE_NAME "hinder-devel")
set(CPACK_RPM_DEVEL_PACKAGE_REQUIRES "hinder = ${PROJECT_VERSION}")

################################################################################
# DEB settings
################################################################################
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_DEBIAN_PACKAGE_SECTION "libs")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")

set(CPACK_DEBIAN_RUNTIME_PACKAGE_NAME "libhinder0")
set(CPACK_DEBIAN_DEVEL_PACKAGE_NAME "libhinder-dev")
set(CPACK_DEBIAN_DEVEL_PACKAGE_DEPENDS "libhinder0 (= ${PROJECT_VERSION})")

################################################################################
# Must come last
################################################################################
include(CPack)
