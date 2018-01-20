include_directories(${Boost_INCLUDE_DIRS})

add_library(regexp regexp.cpp)
add_library(instruction instruction.cpp)
add_library(machine machine.cpp)
