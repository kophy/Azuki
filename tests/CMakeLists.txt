include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES})

add_executable(test_instruction test_instruction.cpp)
target_link_libraries(test_instruction
  instruction
  ${GTEST_LIBS_DIR}/libgtest.a
  ${GTEST_LIBS_DIR}/libgtest_main.a
)

add_test(test_instruction test_instruction)

add_executable(test_machine test_machine.cpp)
target_link_libraries(test_machine
  instruction
  machine
  ${GTEST_LIBS_DIR}/libgtest.a
  ${GTEST_LIBS_DIR}/libgtest_main.a
)

add_test(test_machine test_machine)
