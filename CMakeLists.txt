cmake_minimum_required(VERSION 3.24)
project(type_erasure)

set(CMAKE_CXX_STANDARD 20)

find_package(polymorphic-value CONFIG REQUIRED CONFIGS polymorphic_value-config.cmake)
find_package(proxy CONFIG REQUIRED)

add_executable(inheritance inheritance.cpp)
target_link_libraries(inheritance PRIVATE polymorphic_value::polymorphic_value)

add_executable(manual_type_erasure manual_type_erasure.cpp)
target_link_libraries(manual_type_erasure PRIVATE polymorphic_value::polymorphic_value)

add_executable(proxy proxy.cpp)
target_link_libraries(proxy PRIVATE msft_proxy)
