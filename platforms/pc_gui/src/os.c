#include "os.h"

#include "button_interface.h"
#include "led_interface.h"
#include "spled.h"

void OsInit(void)
{
    ledInterface_init();
    buttonInterface_init();
}

/**
 * One turn of the super loop. The terminal platform owns its loop and sleeps for
 * CONFIG_OS_TASK_PERIOD; here the GUI timer is the tick source, so the period stays
 * a property of the caller.
 */
void OsStep(void)
{
    buttonInterface_update();
    spled();
}
