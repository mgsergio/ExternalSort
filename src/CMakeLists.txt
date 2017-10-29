set(EXECUTABLE_NAME sortfile)

add_subdirectory(io)
add_subdirectory(sort)

add_executable(${EXECUTABLE_NAME} main.cc)
target_link_libraries(
  ${EXECUTABLE_NAME}
  io
)
