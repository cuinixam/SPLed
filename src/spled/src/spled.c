#include "autoconf.h"
#include "spled.h"

#include "power_button.h"
#include "power_signal_processing.h"
#include "light_controller.h"
#include "console_interface.h"
#include "main_control_knob.h"
#if defined(CONFIG_BRIGHTNESS_ADJUSTMENT_IS_ENABLED) && CONFIG_BRIGHTNESS_ADJUSTMENT_IS_ENABLED == 1
#include "brightness_controller.h"
#endif

void spled(void) {
    powerButton();
    powerSignalProcessing();
    mainControlKnob();
#if defined(CONFIG_BRIGHTNESS_ADJUSTMENT_IS_ENABLED) && CONFIG_BRIGHTNESS_ADJUSTMENT_IS_ENABLED == 1
    brightnessController();
#endif
    lightController();
    consoleInterface();
}
