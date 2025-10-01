#ifndef LED_INTERFACE_H
#define LED_INTERFACE_H

#include "rte.h"

// Initialize the LED interface. Called once at startup.
void ledInterface_init(void);
// Control the LED
void ledInterface(void);

#endif /* LED_INTERFACE_H */
