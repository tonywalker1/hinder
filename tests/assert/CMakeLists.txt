################################################################################
# find dependencies
################################################################################
#find_package(Catch2 REQUIRED)

################################################################################
# build project
################################################################################
add_executable(hinder_assert_tests
    main.cpp
#    assert_tests.cpp
)

target_link_libraries(hinder_assert_tests
#    Catch2::Catch2
#    hinder::assert
    )

#include(CTest)
#include(Catch)
#catch_discover_tests(hinder_assert_tests)
