# CMake toolchain file for Arduino Uno ATmega328P

# Set the system name and processor
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

# Specify the cross compiler
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_ASM_COMPILER avr-gcc)
set(CMAKE_AR avr-gcc-ar)
set(CMAKE_RANLIB avr-gcc-ranlib)
set(CMAKE_OBJCOPY avr-objcopy)

# Set executable suffix to .elf for AVR targets
set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")

# MCU configuration
set(MCU atmega328p)
set(F_CPU 16000000L)

# Arduino specific definitions
add_definitions(
    -DARDUINO_AVR_ATMEL_ATMEGA328P_XMINI
    -DF_CPU=${F_CPU}
    -DARDUINO_ARCH_AVR
    -DARDUINO=10607
)

# Compiler flags for C
set(CMAKE_C_FLAGS "-std=gnu11 -mmcu=${MCU} -Os -Wall -MMD -fno-fat-lto-objects -ffunction-sections -fdata-sections ")

# Compiler flags for C++
set(CMAKE_CXX_FLAGS "-std=gnu++11 -mmcu=${MCU} -Os -Wall -MMD -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics")

# Assembler flags
set(CMAKE_ASM_FLAGS "-mmcu=${MCU} -x assembler-with-cpp -MMD")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "-mmcu=${MCU} -Os -Wl,--gc-sections -flto -fuse-linker-plugin -lm")

set(COMPILE_C_FLAGS "-DSPLE_TESTABLE_STATIC=static ")
add_compile_options(
    "$<$<COMPILE_LANGUAGE:C>:${COMPILE_C_FLAGS}>"
)

# Find compiler path and add its path as include directory
find_program(AVR_GCC avr-gcc)
if(NOT AVR_GCC)
    message(FATAL_ERROR "avr-gcc not found. Please install the AVR toolchain and ensure it's in your PATH.")
endif()
message(STATUS "Found avr-gcc: ${AVR_GCC}")
get_filename_component(AVR_GCC_PATH "${AVR_GCC}" PATH)
set(AVR_COMPILER_INCLUDE "${AVR_GCC_PATH}/../avr/include")
add_compile_options(-I${AVR_COMPILER_INCLUDE})
