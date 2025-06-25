#ifndef FLIGHT_CONTROLLER_H
#define FLIGHT_CONTROLLER_H

#include "rte.h"

extern boolean CheckAbort(boolean off_course, boolean abort_commanded, boolean valid_abort_command);

extern boolean CheckAbort_NotTreeLike(boolean abort_commanded, boolean valid_abort_command, boolean off_course);

#endif /* FLIGHT_CONTROLLER_H */
