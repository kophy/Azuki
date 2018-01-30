include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(find_laugh find_laugh.cpp)
target_link_libraries(find_laugh
  azuki
)

add_executable(domain_name domain_name.cpp)
target_link_libraries(domain_name
  azuki
)

add_executable(cellphone cellphone.cpp)
target_link_libraries(cellphone
  azuki
)
