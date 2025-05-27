#ifndef CHECK_ABORT_H
#define CHECK_ABORT_H

#include "rte.h"

extern boolean CheckAbort(boolean off_course, boolean abort_commanded, boolean valid_abort_command);

extern boolean CheckAbort_NotTreeLike(boolean abort_commanded, boolean valid_abort_command, boolean off_course);

#endif /* CHECK_ABORT_H */
