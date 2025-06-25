spl_add_include(${PROJECT_SOURCE_DIR}/components/rte/src)
spl_add_source(src/flight_controller.c)
spl_add_test_source(test/test_flight_controller.cc)
spl_create_component(LONG_NAME "Flight Controller")
