#ifndef BUTTON_INTERFACE_H
#define BUTTON_INTERFACE_H

#include "rte.h"

// Initialize the button interface. Called once at startup.
void buttonInterface_init(void);

boolean ButtonInterfaceIsButtonPressed(KeyCodes key);

#endif /* BUTTON_INTERFACE_H */
