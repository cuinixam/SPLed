#ifndef GUI_ADAPTERS_H
#define GUI_ADAPTERS_H

#include <stdint.h>

// Read the LED colour the application last wrote through led_interface.
void GuiLedGet(uint8_t *red, uint8_t *green, uint8_t *blue);

// Set the pressed state of one button, using the codes from KeyCodes.
void GuiButtonSet(int key_code, int pressed);

#endif /* GUI_ADAPTERS_H */
