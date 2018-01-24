include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(laugh laugh.cpp)
target_link_libraries(laugh
  azuki
)
