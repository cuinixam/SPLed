#include <windows.h>
#include "button_interface.h"

void buttonInterface_init(void)
{
    // No initialization needed for Windows console
}

boolean ButtonInterfaceIsButtonPressed(KeyCodes key)
{
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}
