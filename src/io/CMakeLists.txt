project(libio)

set(
  SRC
  file_stream.cc
  file_stream.hh
  memory_stream.hh
  stream.hh
  string_wrapper.cc
  string_wrapper.hh
)

add_library(io ${SRC})

add_subdirectory(tests)
