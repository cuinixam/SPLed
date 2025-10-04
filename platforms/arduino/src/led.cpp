#include "mcu.h"

extern "C"
{
#include "led_interface.h"
}

// Pin definitions for RGB LED (use PWM-capable pins)
#define RGB_RED_PIN 9
#define RGB_GREEN_PIN 10
#define RGB_BLUE_PIN 11

extern "C"
{
    static RGBColor previousLightValue = {255, 255, 255};

    void ledInterface_init(void)
    {
        previousLightValue.red = 255;
        previousLightValue.green = 255;
        previousLightValue.blue = 255;

        // Initialize the PWM pins for the RGB LED as OUTPUT
        pinMode(RGB_RED_PIN, OUTPUT);
        pinMode(RGB_GREEN_PIN, OUTPUT);
        pinMode(RGB_BLUE_PIN, OUTPUT);

        // Set initial state (all LEDs off)
        analogWrite(RGB_RED_PIN, 0);
        analogWrite(RGB_GREEN_PIN, 0);
        analogWrite(RGB_BLUE_PIN, 0);
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

            // Control the PWM for digital pins for the three colors of the RGB LED
            // Note: For common cathode RGB LEDs, use the value directly
            // For common anode RGB LEDs, use (255 - lightValue.color)
            analogWrite(RGB_RED_PIN, lightValue.red);
            analogWrite(RGB_GREEN_PIN, lightValue.green);
            analogWrite(RGB_BLUE_PIN, lightValue.blue);
        }
    }
}
