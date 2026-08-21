#include "led_interface.h"

#include "gui_adapters.h"
#include "rte.h"

static RGBColor currentLightValue = {0, 0, 0};

void ledInterface_init(void)
{
    currentLightValue.red = 0;
    currentLightValue.green = 0;
    currentLightValue.blue = 0;
}

void ledInterface(void)
{
    RteGetLightValue(&currentLightValue);
}

void GuiLedGet(uint8_t *red, uint8_t *green, uint8_t *blue)
{
    *red = currentLightValue.red;
    *green = currentLightValue.green;
    *blue = currentLightValue.blue;
}
