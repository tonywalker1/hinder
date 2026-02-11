################################################################################
# find dependencies
################################################################################
find_package(GTest REQUIRED)

################################################################################
# build project
################################################################################
add_executable(hinder_misc_tests
    json_string_tests.cpp
    units_tests.cpp
)

target_link_libraries(hinder_misc_tests
    GTest::gtest
    GTest::gtest_main
    hinder::hinder
)

include(CTest)
include(GoogleTest)
gtest_discover_tests(hinder_misc_tests)
