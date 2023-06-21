cmake_minimum_required (VERSION 3.24 FATAL_ERROR)

find_package(Threads REQUIRED)

set(TESTS
  argparse/basic
  argparse/floating_with_suffix
  argparse/integer_with_suffix
  )

set(LIBRARIES
  argparse
  GTest::gtest
  Threads::Threads
  )

configure_tests("core" "${LIBRARIES}" ${TESTS})
