# cmake project definition
cmake_minimum_required(VERSION 3.24.0)
cmake_policy(VERSION 3.24)

# default build kit is prod (when not specified as command line argument)
set(BUILD_KIT prod CACHE STRING "Target Group to build.")

# Include variant's configuration
include(${CMAKE_SOURCE_DIR}/variants/${VARIANT}/config.cmake)

if(BUILD_KIT STREQUAL prod)
    project(${VARIANT} C ASM)
else()
    # C++ project due to GTest usage
    project(${VARIANT} C ASM CXX)
endif()

# Fetch all external dependencies into modules directory
set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/build/modules CACHE INTERNAL "")
set(FETCHCONTENT_QUIET FALSE)
include(FetchContent)

# Global config for spl-core
set(SPL_GTEST_URL https://git.marquardt.de/scm/sple/googletest.git)
set(SPL_GTEST_TAG v1.16.0)

# Include spl-core
include(".venv/Lib/site-packages/spl_core/spl.cmake")

# Include variant specific parts/components definitions
include(${PROJECT_SOURCE_DIR}/variants/${VARIANT}/parts.cmake)

if(BUILD_KIT STREQUAL prod)
    include(".venv/Lib/site-packages/sple_static_analysis_extension/index.cmake")
endif()
