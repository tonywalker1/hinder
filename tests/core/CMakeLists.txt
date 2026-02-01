################################################################################
# find dependencies
################################################################################
find_package(Catch2 REQUIRED)

################################################################################
# build project
################################################################################
add_executable(hinder_core_tests
    main.cpp
    timestamp_tests.cpp
)

target_link_libraries(hinder_core_tests
    Catch2::Catch2
    hinder::hinder
)

include(CTest)
include(Catch)
catch_discover_tests(hinder_core_tests)
