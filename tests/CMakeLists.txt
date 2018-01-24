include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

add_executable(test_utility test_utility.cpp)
target_link_libraries(test_utility
  ${GTEST_BOTH_LIBRARIES}
  utility
)

add_test(test_utility test_utility)

add_executable(test_regexp test_regexp.cpp)
target_link_libraries(test_regexp
  ${GTEST_BOTH_LIBRARIES}
  ${Boost_LIBRARIES}
  regexp
)

add_test(test_regexp test_regexp)

add_executable(test_instruction test_instruction.cpp)
target_link_libraries(test_instruction
  ${GTEST_BOTH_LIBRARIES}
  instruction
)

add_test(test_instruction test_instruction)

add_executable(test_machine test_machine.cpp)
target_link_libraries(test_machine
  machine
  ${GTEST_BOTH_LIBRARIES}
)

add_test(test_machine test_machine)

add_executable(test_azuki test_azuki.cpp)
target_link_libraries(test_azuki
  azuki
  ${GTEST_BOTH_LIBRARIES}
)

add_test(test_azuki test_azuki)
