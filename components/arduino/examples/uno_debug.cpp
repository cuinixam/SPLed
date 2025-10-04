/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

extern "C"
{
#include "rte.h"
#include "button_interface.h"
#include "led_interface.h"
}
#include <Arduino.h>
#include "scheduler.h"

// Pin definitions for RGB LED (use PWM-capable pins)
#define RGB_RED_PIN 9
#define RGB_GREEN_PIN 10
#define RGB_BLUE_PIN 11

void set_led_pin(bool use_pwm, int pin, bool state)
{
    if (use_pwm)
    {
        analogWrite(pin, state ? 200 : 100); // Full brightness or half brightness
    }
    else
    {
        digitalWrite(pin, state ? HIGH : LOW); // Fully on or off
    }
}

void my_loop()
{
    // Adaptive timing - works for both 10ms and 100ms periods
    // For 1 second blink: 1000ms / period = counter_max
    static unsigned int counter = 0;
    static bool led_state = false;
    static bool use_pwm = true;
    static unsigned char key_pressed = 0x00;

#if (defined(CONFIG_OS_TASK_PERIOD) && CONFIG_OS_TASK_PERIOD == 10)
    const unsigned int counter_max = 100; // 1000ms / 10ms = 100
#else
    const unsigned int counter_max = 10; // Default to 100ms period
#endif

    key_pressed = 0x00;
    counter++;
    if (counter >= counter_max)
    {
        counter = 0;
        led_state = !led_state;
        digitalWrite(LED_BUILTIN, led_state ? HIGH : LOW);
    }
    if (counter % 20 == 0) // Every 200ms print status
    {

        // Check if any of the keys are pressed and print their status to the serial monitor
        if (RteIsKeyPressed(KEY_UP))
        {
            Serial.println("KEY_UP is pressed");
            use_pwm = !use_pwm; // Toggle PWM mode
            Serial.print("PWM mode is now ");
            Serial.print((use_pwm ? "ON" : "OFF"));
            Serial.println();
            key_pressed |= 0x01;
        }
        if (RteIsKeyPressed(KEY_DOWN))
        {
            Serial.println("KEY_DOWN is pressed");
            key_pressed |= 0x02;
        }
        if (RteIsKeyPressed(POWER_BUTTON_KEY))
        {
            Serial.println("POWER_BUTTON_KEY is pressed");
            key_pressed |= 0x04;
        }

        // Key 2 and 3 means pin 9 (red) is on
        // Only key 2 means pin 10 (green) is on
        // Only key 3 means pin 11 (blue) is on
        // If use_pwm flag is set, update the LED brightness using analogWrite. Full brightness (255) if key 1 is pressed, else half brightness (128).
        // If use_pwm flag is not set, use digitalWrite to turn the LEDs fully on or off.
        if (key_pressed == 0x06) // Keys 2 and 3
        {
            set_led_pin(use_pwm, RGB_RED_PIN, true);
            set_led_pin(use_pwm, RGB_GREEN_PIN, false);
            set_led_pin(use_pwm, RGB_BLUE_PIN, false);
        }
        else if (key_pressed == 0x02) // Only key 2
        {
            set_led_pin(use_pwm, RGB_RED_PIN, false);
            set_led_pin(use_pwm, RGB_GREEN_PIN, true);
            set_led_pin(use_pwm, RGB_BLUE_PIN, false);
        }
        else if (key_pressed == 0x04) // Only key 3
        {
            set_led_pin(use_pwm, RGB_RED_PIN, false);
            set_led_pin(use_pwm, RGB_GREEN_PIN, false);
            set_led_pin(use_pwm, RGB_BLUE_PIN, true);
        }
    }
}

extern "C"
{
    void Task_Init(void)
    {
        // initialize LED digital pin as an output.
        pinMode(LED_BUILTIN, OUTPUT);
        buttonInterface_init();
        ledInterface_init();
        Serial.begin(9600);
    }

    void Task_100ms(void)
    {
#if (defined(CONFIG_OS_TASK_PERIOD) && CONFIG_OS_TASK_PERIOD == 100)
        my_loop();
#endif
    }

    void Task_10ms(void)
    {
#if (defined(CONFIG_OS_TASK_PERIOD) && CONFIG_OS_TASK_PERIOD == 10)
        my_loop();
#endif
    }
}
