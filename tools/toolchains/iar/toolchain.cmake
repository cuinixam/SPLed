set(IAR_ARM_BASE "$ENV{${COMPILER_NAME}_ROOT}/arm/bin")
set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          cortex-m4f)
# Without that flag CMake is not able to pass test compilation check
set(CMAKE_TRY_COMPILE_TARGET_TYPE   STATIC_LIBRARY)

string(REPLACE "\\" "/" COMPILER_PATH ${IAR_ARM_BASE})
set(IAR_COMP_DIR ${COMPILER_PATH})
set(CMAKE_C_COMPILER ${COMPILER_PATH}/iccarm.exe)
set(CMAKE_CXX_COMPILER ${COMPILER_PATH}/iccarm.exe)
set(CMAKE_LINKER ${COMPILER_PATH}/ilinkarm.exe)
set(CMAKE_ASM_COMPILER ${COMPILER_PATH}/iasmarm.exe)

set(CMAKE_C_FLAGS_DEBUG             "-On -DDEBUG --debug --no_cse --no_unroll --no_inline --no_code_motion --no_tbaa --no_clustering --no_scheduling" CACHE INTERNAL "")
set(CMAKE_ASM_FLAGS_DEBUG           "-r " CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE           "-DNDEBUG -Oh" CACHE INTERNAL "")
set(CMAKE_ASM_FLAGS_RELEASE         "-DNDEBUG " CACHE INTERNAL "")
set(CMAKE_C_FLAGS_MINSIZEREL        "-DNDEBUG -Ohz" CACHE INTERNAL "")
set(CMAKE_ASM_FLAGS_MINSIZEREL      "-DNDEBUG " CACHE INTERNAL "")

# Set compiler argument string for compiler options
set(COMPILER_ARGUMENT_STRING "--")

set(IAR_TARGET_PLATFORM "true")

# Common compiler options
set(CMAKE_CXX_FLAGS "")

if(VARIANT_C_FLAGS)
    set(COMPILE_C_FLAGS ${VARIANT_C_FLAGS})
else()
    set(COMPILE_C_FLAGS --cpu Cortex-M4 --cpu_mode thumb -D__iar__ -DIAR -DBRS_COMP_IAR --fpu None --endian little --header_context -e)
    if(VARIANT_ADDITIONAL_C_FLAGS)
        list(APPEND COMPILE_C_FLAGS ${VARIANT_ADDITIONAL_C_FLAGS})
    endif()
endif()


if(VARIANT_ASM_FLAGS)
    set(COMPILE_ASM_FLAGS ${VARIANT_ASM_FLAGS})
else()
    set(COMPILE_ASM_FLAGS --cpu Cortex-M4 --fpu None --endian little -g -D__iar__ -DIAR -L)
    if(VARIANT_ADDITIONAL_ASM_FLAGS)
        list(APPEND COMPILE_ASM_FLAGS ${VARIANT_ADDITIONAL_ASM_FLAGS})
    endif()
endif()

add_compile_options(
    "$<$<COMPILE_LANGUAGE:C>:${COMPILE_C_FLAGS}>"
    "$<$<COMPILE_LANGUAGE:ASM>:${COMPILE_ASM_FLAGS}>"
)

# Set linker flags
set(CMAKE_EXE_LINKER_FLAGS "--cpu Cortex-M4 --fpu None --force_exceptions")

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Function to set the target properties for a non-autosar project
function(compiler_iar_set_target_link_flags entry_function keep_symbol_list linker_file)
    #set the target properties
    set_target_properties(${LINK_TARGET_NAME} PROPERTIES
        LINK_FLAGS "  --place_holder __checksum,4,.CRCAplication,4 --entry ${entry_function} --keep ${keep_symbol_list} --map ${LINK_FILE_BASENAME}.map --config ${CMAKE_SOURCE_DIR}/PROJECTS/${VARIANT}/sources/${linker_file}"
    )
endfunction(compiler_iar_set_target_link_flags)
