project(ExternalSort CXX)
cmake_minimum_required(VERSION 3.9)

add_compile_options(
  "-Wall"
  "-Werror"
  "-pedantic"
  "-std=c++14"
  "-g"
)

set(SRC main.cc)

add_executable(sortfile ${SRC})
