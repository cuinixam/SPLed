# cmake project definition
cmake_minimum_required(VERSION 3.24.0)
cmake_policy(VERSION 3.24)

# configure the current variant to be build
set(BUILD_KIT prod CACHE STRING "Target Group to build.")

include(${CMAKE_SOURCE_DIR}/variants/${VARIANT}/config.cmake)

if(BUILD_KIT STREQUAL prod)
    project(${VARIANT} C ASM)
else()
    # C++ project due to GTest usage
    project(${VARIANT} C ASM CXX)
endif()

# Fetch all external dependencies into modules directory
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/build/modules CACHE INTERNAL "")
set(FETCHCONTENT_QUIET FALSE)
include(FetchContent)

# Global config for spl-core
set(SPL_GTEST_URL https://git.marquardt.de/scm/sple/googletest.git)
set(SPL_GTEST_TAG v1.16.0)

# Include spl-core
include(".venv/Lib/site-packages/spl_core/spl.cmake")

include(${CMAKE_SOURCE_DIR}/variants/${VARIANT}/parts.cmake)

if(BUILD_KIT STREQUAL prod)
    include(".venv/Lib/site-packages/sple_static_analysis_extension/index.cmake")
endif()

# add all components' include paths to all components
foreach(component_name ${COMPONENT_NAMES})
    if(TARGET ${component_name})
        get_target_property(target_type ${component_name} TYPE)
        if(target_type STREQUAL "INTERFACE_LIBRARY")
            target_include_directories(${component_name} INTERFACE ${target_include_directories__INCLUDES})
        else()
            target_include_directories(${component_name} PUBLIC ${target_include_directories__INCLUDES})
        endif()
    endif()
endforeach()
