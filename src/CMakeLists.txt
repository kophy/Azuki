include_directories(${Boost_INCLUDE_DIRS})

add_library(utility utility.cpp)

add_library(regexp regexp.cpp)

add_library(instruction instruction.cpp)
target_link_libraries(instruction
  regexp
)

add_library(machine machine.cpp)
target_link_libraries(machine
  instruction
)

add_library(azuki azuki.cpp)
target_link_libraries(azuki
  regexp
  machine
  utility
)
