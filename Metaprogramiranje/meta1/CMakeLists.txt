cmake_minimum_required(VERSION 3.13)
project(meta-z3)
add_executable(meta-z3 main.cpp)
set_target_properties(
    meta-z3
    PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
  )

