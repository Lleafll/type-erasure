cmake_minimum_required(VERSION 3.24)
project(type_erasure)

set(CMAKE_CXX_STANDARD 20)

find_package(proxy CONFIG REQUIRED)

add_executable(inheritance inheritance.cpp)
add_executable(manual_type_erasure manual_type_erasure.cpp)
add_executable(proxy proxy.cpp)
target_link_libraries(proxy PRIVATE msft_proxy)
