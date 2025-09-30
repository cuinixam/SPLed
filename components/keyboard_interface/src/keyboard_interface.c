#include <windows.h>
#include "button_interface.h"

boolean ButtonInterfaceIsButtonPressed(KeyCodes key)
{
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}
