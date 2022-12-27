cmake_minimum_required(VERSION 3.24)
project(type_erasure)

set(CMAKE_CXX_STANDARD 20)

set(Boost_NO_WARN_NEW_VERSIONS ON)
find_package(Boost REQUIRED COMPONENTS type_erasure)
find_package(polymorphic-value CONFIG REQUIRED CONFIGS polymorphic_value-config.cmake)
find_package(proxy CONFIG REQUIRED)
include(FetchContent)
FetchContent_Declare(boostextte
        GIT_REPOSITORY https://github.com/boost-ext/te.git
        GIT_TAG 27465847fe489d33a91014488284210f183cb502)
FetchContent_MakeAvailable(boostextte)

add_executable(inheritance inheritance.cpp)
target_link_libraries(inheritance PRIVATE polymorphic_value::polymorphic_value)

add_executable(manual_type_erasure manual_type_erasure.cpp)
target_link_libraries(manual_type_erasure PRIVATE polymorphic_value::polymorphic_value)

add_executable(proxy proxy.cpp)
target_link_libraries(proxy PRIVATE msft_proxy)

add_executable(boost_type_erasure boost_type_erasure.cpp)
target_link_libraries(boost_type_erasure PRIVATE Boost::type_erasure)

add_executable(boostext_te boostext_te.cpp)
target_include_directories(boostext_te PRIVATE ${boostextte_SOURCE_DIR}/include)