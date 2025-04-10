set(CMAKE_C_COMPILER clang CACHE STRING "C Compiler")
set(CMAKE_CXX_COMPILER clang++ CACHE STRING "CXX Compiler")
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER} CACHE STRING "ASM Compiler")

find_program(CMAKE_C_COMPILER_FOUND_PATH ${CMAKE_C_COMPILER})
get_filename_component(CMAKE_C_COMPILER_FOUND_PATH ${CMAKE_C_COMPILER_FOUND_PATH} DIRECTORY)
get_filename_component(CMAKE_C_COMPILER_FOUND_PATH ${CMAKE_C_COMPILER_FOUND_PATH} DIRECTORY)

# needed for VS Code Intellisense
include_directories(BEFORE ${CMAKE_C_COMPILER_FOUND_PATH}/include)
include_directories(BEFORE ${CMAKE_C_COMPILER_FOUND_PATH}/lib/clang/19/include)
include_directories(BEFORE ${CMAKE_C_COMPILER_FOUND_PATH}/x86_64-w64-mingw32/include)

# TODO: clarify why llvm-cov produces invalid gcov files (contain blank lines), releated GCOVR issue: https://github.com/gcovr/gcovr/issues/331
set(GCOVR_ADDITIONAL_OPTIONS --gcov-executable \"llvm-cov gcov\" --gcov-ignore-parse-errors --html-title \"Code Coverage Report \(tool suite: LLVM Clang\)\")
