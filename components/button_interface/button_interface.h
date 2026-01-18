#ifndef BUTTON_INTERFACE_H
#define BUTTON_INTERFACE_H

#include "rte_types.h"

// Initialize the button interface. Called once at startup.
void buttonInterface_init(void);

// Update button states (poll input once per frame)
void buttonInterface_update(void);

boolean ButtonInterfaceIsButtonPressed(KeyCodes key);

#endif /* BUTTON_INTERFACE_H */
