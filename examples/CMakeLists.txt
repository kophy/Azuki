include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(find_laugh find_laugh.cpp)
target_link_libraries(find_laugh
  azuki
)
