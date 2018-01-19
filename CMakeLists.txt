cmake_minimum_required (VERSION 3.10.1)
set (CMAKE_CXX_STANDARD 11)

project (azuki)

enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_subdirectory(src)
add_subdirectory(tests)
