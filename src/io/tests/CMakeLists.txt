project(libio-tests)

set(SRC
  main.cc
  buffered_string_wrapper_test.cc
  file_stream_test.cc
  helpers.hh
  memory_stream_test.cc
  string_view_test.cc
  string_wrapper_test.cc
)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} io)

add_test(${PROJECT_NAME}, ${PROJECT_NAME})
