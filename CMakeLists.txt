cmake_minimum_required (VERSION 3.10.1)
set (CMAKE_CXX_STANDARD 11)

project (azuki)

enable_testing()

add_subdirectory(ext/gtest)
add_subdirectory(src)
add_subdirectory(tests)
