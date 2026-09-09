# Included at the end of the generated variant.cmake (configs id: cmake), once
# the component libraries exist. Everything Zephyr-specific that is not a component.
set(ZEPHYR_APP_SRC ${CMAKE_CURRENT_LIST_DIR}/app/src)

# SPLed's platform toolchain files supply this: `static` in a product build,
# empty in a test build so the tests can reach internal functions.
zephyr_compile_definitions(SPLE_TESTABLE_STATIC=static)
# SPLed components include "autoconf.h" by name; Zephyr generates it here.
zephyr_include_directories(${PROJECT_BINARY_DIR}/include/generated/zephyr)

# Zephyr requires at least one source on `app`; the adapters are yanga components.
target_sources(app PRIVATE ${ZEPHYR_APP_SRC}/main.c)
