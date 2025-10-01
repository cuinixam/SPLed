#include "mcu.h"

extern "C"
{
#include "led_interface.h"
}

extern "C"
{
    static RGBColor previousLightValue = {255, 255, 255};

    void ledInterface_init(void)
    {
        previousLightValue.red = 255;
        previousLightValue.green = 255;
        previousLightValue.blue = 255;

        // TODO: Initialize the digital pins for the RGB LED as OUTPUT
    }

    void ledInterface(void)
    {
        RGBColor lightValue;

        RteGetLightValue(&lightValue);

        // Check if the light value has changed
        if (lightValue.red != previousLightValue.red ||
            lightValue.green != previousLightValue.green ||
            lightValue.blue != previousLightValue.blue)
        {

            // Update the previous light value
            previousLightValue = lightValue;

            // TODO: control the PWN for digital pins for the three colors of the RGB LED
        }
    }
}
