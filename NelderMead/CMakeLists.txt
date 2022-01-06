cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(NelderMead)

add_executable(tests test/main.cpp src/asa047.cpp)

set_target_properties(tests PROPERTIES LINKER_LANGUAGE CXX)
