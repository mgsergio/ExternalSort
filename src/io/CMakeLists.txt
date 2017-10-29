project(libio)

set(
  SRC
  buffered_string_wrapper.cc
  buffered_string_wrapper.hh
  file_stream.cc
  file_stream.hh
  memory_stream.hh
  stream.hh
  string_view.cc
  string_view.hh
  string_wrapper.cc
  string_wrapper.hh
)

add_library(io ${SRC})

add_subdirectory(tests)
