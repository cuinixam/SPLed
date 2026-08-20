#include "led_interface.h"
#include "hw_config.h"

#include "IfxPort.h"

/*
 * The shield carries four single-colour LEDs, not an RGB one, so each channel of
 * the RTE's RGBColor drives its own LED and is thresholded to on/off. The
 * interface asks for a colour, not a brightness, and PWM here would mean bringing
 * up a GTM TOM channel for no gain.
 *
 * The LEDs are wired active-low against 3.3V/5V, so a lit LED is a low pin.
 */
#define LED_ON_THRESHOLD 128u

static void setLed(Ifx_P *port, uint8 pin, uint8 channel)
{
    if (channel >= LED_ON_THRESHOLD)
    {
        IfxPort_setPinLow(port, pin);
    }
    else
    {
        IfxPort_setPinHigh(port, pin);
    }
}

void ledInterface_init(void)
{
    IfxPort_setPinModeOutput(LED_RED_PORT, LED_RED_PIN, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinModeOutput(LED_GREEN_PORT, LED_GREEN_PIN, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinModeOutput(LED_BLUE_PORT, LED_BLUE_PIN, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

    IfxPort_setPinHigh(LED_RED_PORT, LED_RED_PIN);
    IfxPort_setPinHigh(LED_GREEN_PORT, LED_GREEN_PIN);
    IfxPort_setPinHigh(LED_BLUE_PORT, LED_BLUE_PIN);
}

void ledInterface(void)
{
    RGBColor lightValue;

    RteGetLightValue(&lightValue);

    setLed(LED_RED_PORT, LED_RED_PIN, lightValue.red);
    setLed(LED_GREEN_PORT, LED_GREEN_PIN, lightValue.green);
    setLed(LED_BLUE_PORT, LED_BLUE_PIN, lightValue.blue);
}
