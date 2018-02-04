find_package(Boost REQUIRED COMPONENTS python system)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})

PYTHON_ADD_MODULE(pyazuki pyazuki.cpp)
target_link_libraries(pyazuki
  azuki
  ${Boost_LIBRARIES}
  ${PYTHON_LIBRARIES}
)

FILE(COPY demo.py DESTINATION .)
