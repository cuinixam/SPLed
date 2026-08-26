#include "spled_api.h"

#include "autoconf.h"
#include "gui_adapters.h"
#include "os.h"
#include "rte.h"

void spled_init(void)
{
    OsInit();
}

void spled_step(void)
{
    OsStep();
}

void spled_get_led(uint8_t *red, uint8_t *green, uint8_t *blue)
{
    GuiLedGet(red, green, blue);
}

void spled_set_button(int key_code, int pressed)
{
    GuiButtonSet(key_code, pressed);
}

int spled_task_period_ms(void)
{
    return CONFIG_OS_TASK_PERIOD;
}

int spled_get_power_state(void)
{
    return RteGetPowerState();
}

int spled_get_main_knob_value(void)
{
    return RteGetMainKnobValue();
}

int spled_get_brightness(void)
{
    return (int)RteGetBrightnessValue();
}
