# Included by the root CMakeLists.txt before project(), which is where Zephyr
# requires find_package(Zephyr): it sets up the toolchain, devicetree and Kconfig.
find_package(Zephyr REQUIRED HINTS ${ZEPHYR_BASE})
