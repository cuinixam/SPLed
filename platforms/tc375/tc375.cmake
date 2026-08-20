# CMake toolchain file for the Infineon AURIX TC375 on KIT-A2G-TC375-ARD-SB
# (Hitex ShieldBuddy), built bare metal with the open-source tricore-elf GCC.

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR tricore)

set(CMAKE_C_COMPILER tricore-elf-gcc)
set(CMAKE_CXX_COMPILER tricore-elf-g++)
set(CMAKE_ASM_COMPILER tricore-elf-gcc)
set(CMAKE_AR tricore-elf-gcc-ar)
set(CMAKE_RANLIB tricore-elf-gcc-ranlib)
set(CMAKE_OBJCOPY tricore-elf-objcopy)

# The compiler cannot link a hosted executable without a linker script, so CMake's
# compiler-identification step must stop at a static library.
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")

# Infineon's startup and iLLD sources select their #pragma section placement and
# intrinsics from compiler-detection macros. HighTec's GNU-C defines __HIGHTEC__;
# this toolchain does not, and without it the sources take a path that does not
# build. Forcing the define is the whole workaround.
add_compile_definitions(__HIGHTEC__)

# TC1.6.2P, the TC375's core.
set(TRICORE_CORE_FLAGS "-mtc162")

# -fstrict-volatile-bitfields is correctness, not style: iLLD accesses SFRs
# through volatile bitfields and relies on them being read and written whole.
set(CMAKE_C_FLAGS "${TRICORE_CORE_FLAGS} -Wall -fno-common -fstrict-volatile-bitfields -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_ASM_FLAGS "${TRICORE_CORE_FLAGS} -x assembler-with-cpp")

# The linker script is not named here. It ships with the BSP dependency, whose
# install path is revision-scoped and unknown to a toolchain file, so
# TricoreLinkerScriptCMakeGenerator in steps.py resolves it and emits the -T.
set(CMAKE_EXE_LINKER_FLAGS
    "-nostartfiles ${TRICORE_CORE_FLAGS} -Wl,--gc-sections -Wl,--extmap=a -Wl,-n")

set(COMPILE_C_FLAGS "-DSPLE_TESTABLE_STATIC=static ")
add_compile_options(
    "$<$<COMPILE_LANGUAGE:C>:${COMPILE_C_FLAGS}>"
)

find_program(TRICORE_GCC tricore-elf-gcc)
if(NOT TRICORE_GCC)
    message(FATAL_ERROR "tricore-elf-gcc not found. Run the install pipeline so poks puts the toolchain on PATH.")
endif()
message(STATUS "Found tricore-elf-gcc: ${TRICORE_GCC}")
