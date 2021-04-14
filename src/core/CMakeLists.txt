################################################################################
# find dependencies
################################################################################
find_package(date REQUIRED)
find_package(fmt REQUIRED)

################################################################################
# build subproject
################################################################################
add_library(core SHARED
    timestamp.cpp
    )
set_target_properties(core PROPERTIES OUTPUT_NAME "hinder_core")
add_library(hinder::core ALIAS core)

target_include_directories(core
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>
    )

target_link_libraries(core
    PUBLIC date-tz
    PUBLIC fmt
    )

################################################################################
# install project
################################################################################
install(TARGETS core
    EXPORT hinder_coreTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/hinder/core
    DESTINATION include/hinder
)
install(EXPORT hinder_coreTargets
    FILE hinder_coreTargets.cmake
    NAMESPACE hinder::
    DESTINATION lib/cmake/hinder_core
    )
install(
    FILES ${PROJECT_SOURCE_DIR}/cmake/hinder_coreConfig.cmake
    DESTINATION lib/cmake/hinder_core
)
