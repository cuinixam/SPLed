#include "autoconf.h"
#include "spled.h"
#include "led_interface.h"
#include "button_interface.h"
#include <unistd.h>

// External declaration for update function (usually in button_interface.h)
void buttonInterface_update(void);

static void Hal_Init(void)
{
    ledInterface_init();
    buttonInterface_init();
}

static void OsTask(void)
{
    // Update input state first
    buttonInterface_update();

    spled();
    // Delay in microseconds
    usleep(CONFIG_OS_TASK_PERIOD * 1000); // CONFIG_OS_TASK_PERIOD is in milliseconds
}

void OsRun(void)
{
    Hal_Init();
    while (1)
    {
        OsTask();
    }
}
