cmake_minimum_required(VERSION 3.20)
include("${CMAKE_BINARY_DIR}/config.cmake")
# A platform whose build system must run before project() (Zephyr) names its
# setup file in a `vars` config; every other platform leaves it unset.
if(PLATFORM_CMAKE_SETUP)
  include("${CMAKE_SOURCE_DIR}/${PLATFORM_CMAKE_SETUP}")
endif()
project(${VARIANT} C ASM CXX)
include("${CMAKE_BINARY_DIR}/variant.cmake")
