cmake_minimum_required(VERSION 3.20)

project(limit_order_book
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(LOB_BUILD_TESTS "Build unit tests" ON)
option(LOB_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

add_library(lob
    src/order_book.cpp
    src/matching_engine.cpp
)

target_include_directories(lob
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(lob PUBLIC cxx_std_20)

target_compile_options(lob PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

if (LOB_ENABLE_SANITIZERS)
    target_compile_options(lob PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(lob PRIVATE -fsanitize=address,undefined)
endif()

if (LOB_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()