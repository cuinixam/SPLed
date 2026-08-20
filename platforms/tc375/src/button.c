#include "button_interface.h"
#include "hw_config.h"

#include "IfxPort.h"

/*
 * The shield's buttons pull their pin to ground when pressed, so a pressed button
 * reads low and the pins are configured with a pull-up.
 *
 * Unlike the pc_terminal adapter, nothing here has to fake a hold time: the
 * buttons latch physically for as long as they are held, which is what the
 * debounce in the components above already expects. buttonInterface_update()
 * samples all three once per cycle so a single cycle sees a consistent state.
 */

static boolean upPressed;
static boolean downPressed;
static boolean powerPressed;

static boolean readButton(Ifx_P *port, uint8 pin)
{
    return IfxPort_getPinState(port, pin) ? FALSE : TRUE;
}

void buttonInterface_init(void)
{
    IfxPort_setPinModeInput(BUTTON_UP_PORT, BUTTON_UP_PIN, IfxPort_InputMode_pullUp);
    IfxPort_setPinModeInput(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN, IfxPort_InputMode_pullUp);
    IfxPort_setPinModeInput(BUTTON_POWER_PORT, BUTTON_POWER_PIN, IfxPort_InputMode_pullUp);

    upPressed = FALSE;
    downPressed = FALSE;
    powerPressed = FALSE;
}

void buttonInterface_update(void)
{
    upPressed = readButton(BUTTON_UP_PORT, BUTTON_UP_PIN);
    downPressed = readButton(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN);
    powerPressed = readButton(BUTTON_POWER_PORT, BUTTON_POWER_PIN);
}

boolean ButtonInterfaceIsButtonPressed(KeyCodes key)
{
    switch (key)
    {
    case KEY_UP:
        return upPressed;
    case KEY_DOWN:
        return downPressed;
    case POWER_BUTTON_KEY:
        return powerPressed;
    default:
        return FALSE;
    }
}
