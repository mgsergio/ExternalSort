project(ExternalSort CXX)
cmake_minimum_required(VERSION 3.9)

add_compile_options(
  "-Wall"
  "-Werror"
  "-pedantic"
  "-std=c++14"
)

include_directories(${PROJECT_SOURCE_DIR}/3party/)

enable_testing()
add_subdirectory(src)
