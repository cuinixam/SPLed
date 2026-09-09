# Included at the end of the generated variant.cmake (configs id: cmake), once
# the component libraries exist. Everything Zephyr-specific that is not a component.
set(ZEPHYR_APP_SRC ${CMAKE_CURRENT_LIST_DIR}/app/src)

# SPLed's platform toolchain files supply this: `static` in a product build,
# empty in a test build so the tests can reach internal functions.
zephyr_compile_definitions(SPLE_TESTABLE_STATIC=static)
# SPLed components include "autoconf.h" by name; Zephyr generates it here.
zephyr_include_directories(${PROJECT_BINARY_DIR}/include/generated/zephyr)

target_sources(app PRIVATE ${ZEPHYR_APP_SRC}/main.c ${ZEPHYR_APP_SRC}/button_adapter.c)

# The platform adapters, picked by what the board's devicetree offers rather
# than by the board name: an RGB LED on a data line goes through led_strip, a
# plain on/off LED through gpio.
target_sources_ifdef(CONFIG_LED_STRIP app PRIVATE ${ZEPHYR_APP_SRC}/led_adapter_ws2812.c)
target_sources_ifndef(CONFIG_LED_STRIP app PRIVATE ${ZEPHYR_APP_SRC}/led_adapter.c)
# Both boards: on native_sim it is the only way to press a button, on the
# board it is the one that needs no wiring.
target_sources_ifdef(CONFIG_SHELL app PRIVATE ${ZEPHYR_APP_SRC}/buttons_shell.c)
