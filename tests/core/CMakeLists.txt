################################################################################
# find dependencies
################################################################################
find_package(GTest REQUIRED)

################################################################################
# build project
################################################################################
add_executable(hinder_core_tests
    timestamp_tests.cpp
)

target_link_libraries(hinder_core_tests
    GTest::gtest
    GTest::gtest_main
    hinder::hinder
)

include(CTest)
include(GoogleTest)
gtest_discover_tests(hinder_core_tests)
