include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

target_compile_options(gtest PRIVATE
    -Wno-character-conversion
) # to remove the 6 conversion warnings

include(GoogleTest)

# Version Tests
add_executable(version_tests
    version_tests.cpp
)

target_link_libraries(version_tests
    PRIVATE
        network_influence
        GTest::gtest_main
)

gtest_discover_tests(version_tests)

# Graph Tests
add_executable(graph_tests
    graph_tests.cpp
)

target_link_libraries(graph_tests
    PRIVATE
        network_influence
        GTest::gtest_main
)

gtest_discover_tests(graph_tests)

# Independent Cascade Tests
add_executable(independent_cascade_tests
    independent_cascade_tests.cpp
)

target_link_libraries(independent_cascade_tests
    PRIVATE
        network_influence
        GTest::gtest_main
)

gtest_discover_tests(independent_cascade_tests)
