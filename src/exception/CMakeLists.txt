################################################################################
# build subproject
################################################################################
add_library(exception SHARED
    exception_config.cpp
    string.cpp
    )
set_target_properties(exception PROPERTIES OUTPUT_NAME "hinder_exception")
add_library(hinder::exception ALIAS exception)

target_include_directories(exception
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>
    )

target_link_libraries(exception
    PUBLIC hinder::core
    )

################################################################################
# install project
################################################################################
install(TARGETS exception
    EXPORT hinder_exceptionTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/hinder/exception
    DESTINATION include/hinder
)
install(EXPORT hinder_exceptionTargets
    FILE hinder_exceptionTargets.cmake
    NAMESPACE hinder::
    DESTINATION lib/cmake/hinder_exception
    )
install(
    FILES ${PROJECT_SOURCE_DIR}/cmake/hinder_exceptionConfig.cmake
    DESTINATION lib/cmake/hinder_exception
)
