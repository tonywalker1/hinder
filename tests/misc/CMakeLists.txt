################################################################################
# find dependencies
################################################################################
find_package(Catch2 REQUIRED)
find_package(fmt REQUIRED)

################################################################################
# build project
################################################################################
add_executable(hinder_misc_tests
    main.cpp
    units_tests.cpp
    )

target_link_libraries(hinder_misc_tests
    Catch2::Catch2
    fmt
    hinder::misc
    )

include(CTest)
include(Catch)
catch_discover_tests(hinder_misc_tests)
