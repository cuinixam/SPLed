#include "mcu.h"

extern "C"
{
#include "button_interface.h"
}

// Pin mappings for each key
#define PIN_KEY_UP 2
#define PIN_KEY_DOWN 3
#define PIN_POWER_BUTTON 4

// Internal pullup configuration - buttons are active LOW
static void initializePins()
{
    static bool initialized = false;
    if (!initialized)
    {
        pinMode(PIN_KEY_UP, INPUT_PULLUP);
        pinMode(PIN_KEY_DOWN, INPUT_PULLUP);
        pinMode(PIN_POWER_BUTTON, INPUT_PULLUP);
        initialized = true;
    }
}

extern "C"
{
    void buttonInterface_init(void)
    {
        initializePins();
    }

    boolean ButtonInterfaceIsButtonPressed(KeyCodes key)
    {
        initializePins();

        int pin = -1;

        // Map KeyCodes to corresponding digital pins
        switch (key)
        {
        case KEY_UP:
            pin = PIN_KEY_UP;
            break;
        case KEY_DOWN:
            pin = PIN_KEY_DOWN;
            break;
        case POWER_BUTTON_KEY:
            pin = PIN_POWER_BUTTON;
            break;
        default:
            return FALSE; // Unknown key
        }

        // Read pin state - buttons are active LOW with internal pullup
        // So pressed = LOW (0), not pressed = HIGH (1)
        return (digitalRead(pin) == LOW) ? TRUE : FALSE;
    }
}
