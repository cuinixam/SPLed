# Same host toolchain as pc_terminal, included rather than copied so the two PC
# platforms cannot drift apart.
include("${CMAKE_CURRENT_LIST_DIR}/../pc_terminal/clang.cmake")

# The variant is linked into a shared library, so every object in it has to be
# position independent.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
