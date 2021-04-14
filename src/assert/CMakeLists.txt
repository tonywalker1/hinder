################################################################################
# find dependencies
################################################################################
find_package(date REQUIRED)
find_package(fmt REQUIRED)

################################################################################
# build project
################################################################################
add_library(assert SHARED
    assert_config.cpp
    )
set_target_properties(assert PROPERTIES OUTPUT_NAME "hinder_assert")
add_library(hinder::assert ALIAS assert)

target_include_directories(assert
    INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    INTERFACE $<INSTALL_INTERFACE:include>
    )

target_link_libraries(assert
    PUBLIC date-tz
    PUBLIC fmt
    PUBLIC hinder::core
    )

################################################################################
# install project
################################################################################
install(TARGETS assert
    EXPORT hinder_assertTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/hinder/assert
    DESTINATION include/hinder
)
install(EXPORT hinder_assertTargets
    FILE hinder_assertTargets.cmake
    NAMESPACE hinder::
    DESTINATION lib/cmake/hinder_assert
    )
install(
    FILES ${PROJECT_SOURCE_DIR}/cmake/hinder_assertConfig.cmake
    DESTINATION lib/cmake/hinder_assert
)
