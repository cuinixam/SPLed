#include "autoconf.h"
#include "spled.h"
#include <unistd.h>

static void OsTask(void)
{
    spled();
    // Delay in microseconds
    usleep(CONFIG_OS_TASK_PERIOD * 1000); // CONFIG_OS_TASK_PERIOD is in milliseconds
}

void OsRun(void)
{
    while (1)
    {
        OsTask();
    }
}
