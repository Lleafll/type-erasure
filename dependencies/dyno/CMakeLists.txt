# Copyright Louis Dionne 2017
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.9)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(Dyno VERSION 0.0.1 LANGUAGES CXX)

##############################################################################
# Setup the `dyno` library target.
##############################################################################
add_library(dyno INTERFACE)
add_library(Dyno::dyno ALIAS dyno)
target_compile_features(dyno INTERFACE cxx_std_17)
target_include_directories(dyno INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
find_package(Boost REQUIRED)
target_link_libraries(dyno INTERFACE Boost::boost)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-Wno-gnu-string-literal-operator-template" DYNO_HAS_WNO_GNU_STRING_UDL)
if (DYNO_HAS_WNO_GNU_STRING_UDL)
  target_compile_options(dyno INTERFACE -Wno-gnu-string-literal-operator-template)
endif()


##############################################################################
# Setup the installation target for dyno and the related exports.
##############################################################################
install(TARGETS dyno
  EXPORT dyno-targets
  INCLUDES DESTINATION include
)
install(EXPORT dyno-targets
  FILE dyno-targets.cmake
  NAMESPACE Dyno::
  DESTINATION lib/cmake/dyno
)
install(FILES     cmake/dyno-config.cmake DESTINATION lib/cmake/dyno)
install(FILES     include/dyno.hpp        DESTINATION include)
install(DIRECTORY include/dyno            DESTINATION include FILES_MATCHING PATTERN "*.hpp")


##############################################################################
# Setup unit tests, examples, etc.
##############################################################################
# Properties common to unit tests and examples:
function(dyno_set_common_properties target)
  target_link_libraries(${target} PRIVATE Dyno::dyno)
  set_target_properties(${target} PROPERTIES CXX_EXTENSIONS NO)
  macro(setflag testname flag)
      check_cxx_compiler_flag(${flag} ${testname})
      if (${testname})
          target_compile_options(${target} PRIVATE ${flag})
      endif()
  endmacro()
  setflag(DYNO_HAS_FTEMPLATE_BACKTRACE_LIMIT -ftemplate-backtrace-limit=0) # helps debugging
  setflag(DYNO_HAS_PEDANTIC                  -pedantic)
  setflag(DYNO_HAS_WALL                      -Wall)
  setflag(DYNO_HAS_WEXTRA                    -Werror)
  setflag(DYNO_HAS_WEXTRA                    -Wextra)
endfunction()

# Return the target name associated to a source file. If the path of the
# source file relative from the root of the project is `path/to/file.cpp`,
# the target name associated to it will be `path.to.file`.
function(dyno_get_target_name out file)
  if (NOT IS_ABSOLUTE "${file}")
    set(file "${CMAKE_CURRENT_SOURCE_DIR}/${file}")
  endif()
  file(RELATIVE_PATH _relative "${PROJECT_SOURCE_DIR}" "${file}")
  string(REPLACE ".cpp" "" _name "${_relative}")
  string(REGEX REPLACE "/" "." _name "${_name}")
  set(${out} "${_name}" PARENT_SCOPE)
endfunction()

# Setup the `check` convenience target, which??is equivalent to examples + tests.
add_custom_target(check COMMENT "Build and run all the tests and examples.")

# Setup subdirectories
enable_testing()
add_subdirectory(benchmark)
add_subdirectory(example)
add_subdirectory(test)
