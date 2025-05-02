#ifndef BRIGHTNESS_CONTROLLER_H
#define BRIGHTNESS_CONTROLLER_H

#include "autoconf.h"

#ifdef CONFIG_BRIGHTNESS_ADJUSTMENT_AUTOMATIC

typedef struct
{
    unsigned int ticksCounter;
    const unsigned int period;
    const unsigned int halfPeriod;
    const unsigned int maxBrightness;
    const unsigned int minBrightness;
} BrightnessAdjustmentData;

#endif /* CONFIG_BRIGHTNESS_ADJUSTMENT_AUTOMATIC */

void brightnessController(void);

#endif /* BRIGHTNESS_CONTROLLER_H */
