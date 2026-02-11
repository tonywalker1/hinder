################################################################################
# find dependencies
################################################################################
find_package(GTest REQUIRED)

################################################################################
# build project
################################################################################
add_executable(hinder_exception_tests
    exception_tests.cpp
)

target_link_libraries(hinder_exception_tests
    GTest::gtest
    GTest::gtest_main
    hinder::hinder
)

include(CTest)
include(GoogleTest)
gtest_discover_tests(hinder_exception_tests)
