cmake_minimum_required (VERSION 3.10)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "-g")

project (azuki)

enable_testing()

find_package (GTest REQUIRED)
find_package (Boost REQUIRED COMPONENTS system)
find_package (PythonInterp REQUIRED)
find_package (PythonLibs REQUIRED)

add_subdirectory (src)
add_subdirectory (tests)
add_subdirectory (examples)
add_subdirectory (python)
