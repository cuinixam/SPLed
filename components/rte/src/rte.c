#include "rte.h"
#include "autoconf.h"
#include "button_interface.h"

static PowerState currentPowerState = POWER_STATE_OFF;
static boolean powerKeyPressedEvent = FALSE;
static RGBColor lightValue = {
    .red = 0,
    .green = 0,
    .blue = 0,
};
static percentage_t mainKnobValue = 50;
static unsigned int brightnessValue = 0;

#ifdef CONFIG_BRIGHTNESS_ADJUSTMENT_PERIOD
static unsigned int brightnessAdjustmentCounter = 0;
#endif

// Flight Controller RTE variables
static boolean offCourse = FALSE;
static boolean abortCommanded = FALSE;
static boolean validAbortCommand = FALSE;
static boolean selfDestructState = FALSE;

void RteSetPowerState(PowerState state)
{
    currentPowerState = state;
}

PowerState RteGetPowerState(void)
{
    return currentPowerState;
}

void RteSetPowerKeyPressedEvent(boolean value)
{
    powerKeyPressedEvent = value;
}

boolean RteGetPowerKeyPressedEvent()
{
    return powerKeyPressedEvent;
}

void RteSetLightValue(RGBColor value)
{
    lightValue = value;
}

void RteGetLightValue(RGBColor *value)
{
    *value = lightValue;
}

boolean RteIsKeyPressed(KeyCodes key)
{
    return ButtonInterfaceIsButtonPressed(key);
}

void RteSetMainKnobValue(percentage_t value)
{
    if (value > 100)
    {
        mainKnobValue = 100;
    }
    else
    {
        mainKnobValue = value;
    }
}

percentage_t RteGetMainKnobValue(void)
{
    return mainKnobValue;
}

void RteSetBrightnessValue(brightness_t value)
{
    brightnessValue = value;
}

brightness_t RteGetBrightnessValue(void)
{
    return brightnessValue;
}

#ifdef CONFIG_BRIGHTNESS_ADJUSTMENT_PERIOD
void RteSetBrightnessAdjustmentCounter(unsigned int counter)
{
    brightnessAdjustmentCounter = counter;
}

void RteGetBrightnessAdjustmentCounter(unsigned int *counter)
{
    *counter = brightnessAdjustmentCounter;
}
#endif // CONFIG_BRIGHTNESS_ADJUSTMENT_PERIOD

void RteGetOffCourse(boolean *value)
{
    if (value != NULL)
    {
        *value = offCourse;
    }
}

boolean RteGetAbortCommanded(void)
{
    return abortCommanded;
}

boolean RteGetValidAbortCommand(void)
{
    return validAbortCommand;
}

void RteSetSelfDestructState(boolean state)
{
    selfDestructState = state;
}
