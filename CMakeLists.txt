cmake_minimum_required (VERSION 3.10.1)
set (CMAKE_CXX_STANDARD 17)

project (azuki)

enable_testing()

find_package(GTest REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)
