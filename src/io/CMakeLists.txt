project(libio)

set(
  SRC
  memory_stream.hh
  stream.hh
  string_wrapper.cc
  string_wrapper.hh
)

add_library(io ${SRC})

add_subdirectory(tests)
