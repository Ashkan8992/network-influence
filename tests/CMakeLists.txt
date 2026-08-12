include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(version_tests
    version_tests.cpp
)

target_link_libraries(version_tests
    PRIVATE
        network_influence
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(version_tests)