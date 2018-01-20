include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

add_executable(test_regexp test_regexp.cpp)
target_link_libraries(test_regexp
  ${GTEST_BOTH_LIBRARIES}
  ${Boost_LIBRARIES}
  regexp
)

add_test(test_regexp test_regexp)

add_executable(test_instruction test_instruction.cpp)
target_link_libraries(test_instruction
  instruction
  ${GTEST_BOTH_LIBRARIES}
)

add_test(test_instruction test_instruction)

add_executable(test_machine test_machine.cpp)
target_link_libraries(test_machine
  instruction
  machine
  ${GTEST_BOTH_LIBRARIES}
)

add_test(test_machine test_machine)
