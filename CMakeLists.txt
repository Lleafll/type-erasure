cmake_minimum_required(VERSION 3.24)
project(type_erasure)

set(CMAKE_CXX_STANDARD 20)

add_executable(inheritance inheritance.cpp)
add_executable(manual_type_erasure manual_type_erasure.cpp)