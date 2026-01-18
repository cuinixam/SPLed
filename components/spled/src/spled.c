#include "autoconf.h"
#include "spled.h"

#include "power_button.h"
#include "power_signal_processing.h"
#include "light_controller.h"
#include "main_control_knob.h"
#include "led_interface.h"
#include "button_interface.h"
#if defined(CONFIG_BRIGHTNESS_ADJUSTMENT_ENABLED) && CONFIG_BRIGHTNESS_ADJUSTMENT_ENABLED == 1
#include "brightness_controller.h"
#endif

void spled(void)
{
    powerButton();
    powerSignalProcessing();
    mainControlKnob();
#if defined(CONFIG_BRIGHTNESS_ADJUSTMENT_ENABLED) && CONFIG_BRIGHTNESS_ADJUSTMENT_ENABLED == 1
    brightnessController();
#endif
    lightController();
    ledInterface();
}

void Task_Init(void)
{
    // Application-level initialization if needed
}

void Task_100ms(void)
{
#if (defined(CONFIG_OS_TASK_PERIOD) && CONFIG_OS_TASK_PERIOD == 100)
    // Call the main application function every 100ms
    spled();
#endif
}

void Task_10ms(void)
{
#if (defined(CONFIG_OS_TASK_PERIOD) && CONFIG_OS_TASK_PERIOD == 10)
    // Call the main application function every 10ms
    spled();
#endif
}
