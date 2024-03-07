cmake_minimum_required (VERSION 3.24 FATAL_ERROR)
project (timer VERSION 0.1 LANGUAGES C CXX ASM)

# Make including this file idempotent
#
if(TARGET argparse OR TARGET argparse::argparse)
  return()
endif()

# If this is the current project.
#
if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
  # Laod the cmake recipes and utilities.
  #
  include(${CMAKE_CURRENT_LIST_DIR}/cmake/load_cmake_helpers.cmake)

  # Options for generating tests and documentation
  #
  option(ARGPARSE_TEST "Generate the tests." ON)
  option(ARGPARSE_DOCS "Generate the docs." OFF)

  # compile_commands.json
  #
  symlink_compile_commands()

  # Convenience targets for git operations
  #
  make_submodule_pull_target()

  # Setup compilation parameters
  #
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  
  add_compile_options(-Wall)
  add_compile_options(-g)
  add_compile_options(-O)

else()
  option(ARGPARSE_TEST "Generate the tests." OFF)
  option(ARGPARSE_DOCS "Generate the docs." OFF)
endif()

# Put executables in the top-level binary directory
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Output configuration
#
message("-- argparse: Configuration: ${CMAKE_CURRENT_SOURCE_DIR}")
message("-- argparse: Included from: ${CMAKE_SOURCE_DIR}")
message("-- argparse: Install prefix: ${CMAKE_INSTALL_PREFIX}")
message("-- argparse: test ${ARGPARSE_TEST}")
message("-- argparse: docs ${ARGPARSE_DOCS}")

# Add our dependencies
#
add_repo(tuple)
add_repo(lexical_cast)

# Build the library
#
set(SOURCES
  detail/base
  detail/context
  detail/error
  )

set(FILES)
foreach(SOURCE ${SOURCES})
  list(APPEND FILES "src/core/argparse/${SOURCE}.cpp")
endforeach()

add_library(argparse ${FILES})
add_library(argparse::argparse ALIAS argparse)

# Define the header files for this library.
#
file(GLOB_RECURSE PUBLIC_INCLUDE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)
target_sources(argparse INTERFACE FILE_SET HEADERS BASE_DIRS include FILES ${PUBLIC_INCLUDE_FILES})

target_include_directories(argparse PUBLIC include)
target_link_libraries(argparse PUBLIC lexical_cast::lexical_cast tuple::tuple)

foreach(prog
    argparse0
    argparse1
    argparse_flag
    argparse_optional
    argparse_value
    argparse_values
    )
  add_executable(${prog} src/tools/${prog}.cpp)
  target_link_libraries(${prog} argparse::argparse)
endforeach()

# Optionally configure the tests
#
if(ARGPARSE_TEST)
 add_gtest()
 add_subdirectory(test)
endif()

# Optionally configure the documentation
#
if(ARGPARSE_DOCS)
  add_subdirectory(docs)
endif()

# Installation
#
#install_config(argparse)
