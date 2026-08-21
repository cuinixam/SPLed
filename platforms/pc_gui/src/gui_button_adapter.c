#include "button_interface.h"

#include "gui_adapters.h"

static boolean powerPressed = FALSE;
static boolean upPressed = FALSE;
static boolean downPressed = FALSE;

void buttonInterface_init(void)
{
    powerPressed = FALSE;
    upPressed = FALSE;
    downPressed = FALSE;
}

/**
 * The terminal adapter has to fake a hardware hold here, because a keystroke is an
 * event and the debounce in power_button expects a level. The GUI holds the button
 * down for as long as the mouse button is down, so there is nothing to simulate.
 */
void buttonInterface_update(void)
{
}

boolean ButtonInterfaceIsButtonPressed(KeyCodes key)
{
    boolean result;

    switch (key)
    {
    case POWER_BUTTON_KEY:
        result = powerPressed;
        break;
    case KEY_UP:
        result = upPressed;
        break;
    case KEY_DOWN:
        result = downPressed;
        break;
    default:
        result = FALSE;
        break;
    }

    return result;
}

void GuiButtonSet(int key_code, int pressed)
{
    const boolean state = pressed ? TRUE : FALSE;

    switch (key_code)
    {
    case POWER_BUTTON_KEY:
        powerPressed = state;
        break;
    case KEY_UP:
        upPressed = state;
        break;
    case KEY_DOWN:
        downPressed = state;
        break;
    default:
        break;
    }
}
