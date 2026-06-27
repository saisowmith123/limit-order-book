include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

add_executable(order_book_tests
    order_book_tests.cpp
)

target_compile_features(order_book_tests PRIVATE cxx_std_20)

target_link_libraries(order_book_tests
    PRIVATE
        lob
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(order_book_tests)