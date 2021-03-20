################################################################################
# find dependencies
################################################################################

################################################################################
# build project
################################################################################
add_library(misc INTERFACE)
add_library(hinder::misc ALIAS misc)

target_include_directories(misc
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )

################################################################################
# install project
################################################################################
install(TARGETS misc
    EXPORT hinder_miscTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/hinder/misc
    DESTINATION include/hinder
)
install(EXPORT hinder_miscTargets
    FILE hinder_miscTargets.cmake
    NAMESPACE hinder::
    DESTINATION lib/cmake/hinder_misc
    )
install(
    FILES ${PROJECT_SOURCE_DIR}/cmake/hinder_miscConfig.cmake
    DESTINATION lib/cmake/hinder_misc
)
