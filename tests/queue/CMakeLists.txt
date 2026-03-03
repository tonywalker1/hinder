################################################################################
# find dependencies
################################################################################
find_package(GTest REQUIRED)

################################################################################
# build project
################################################################################
add_executable(hinder_queue_tests
    queue_tests.cpp
)

target_link_libraries(hinder_queue_tests
    GTest::gtest
    GTest::gtest_main
    hinder::hinder
)

include(CTest)
include(GoogleTest)
gtest_discover_tests(hinder_queue_tests)
