#ifndef FLIGHT_CONTROLLER_H
#define FLIGHT_CONTROLLER_H

#include "rte.h"

extern boolean CheckAbort(boolean off_course, boolean abort_commanded, boolean valid_abort_command);

void flightController(void);

#endif /* FLIGHT_CONTROLLER_H */
