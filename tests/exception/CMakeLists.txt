################################################################################
# find dependencies
################################################################################
find_package(Catch2 REQUIRED)

################################################################################
# build project
################################################################################
add_executable(hinder_exception_tests
    main.cpp
    exception_tests.cpp
)

target_link_libraries(hinder_exception_tests
    Catch2::Catch2
    hinder::hinder
)

include(CTest)
include(Catch)
catch_discover_tests(hinder_exception_tests)
