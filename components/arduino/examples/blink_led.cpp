/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

extern "C"
{
#include "rte.h"
}
#include <Arduino.h>
#include "scheduler.h"

void blink_led()
{
    // Adaptive timing - works for both 10ms and 100ms periods
    // For 1 second blink: 1000ms / period = counter_max
    static unsigned int counter = 0;
    static bool led_state = false;

#if (defined(CONFIG_OS_TASK_PERIOD) && CONFIG_OS_TASK_PERIOD == 10)
    const unsigned int counter_max = 100; // 1000ms / 10ms = 100
#else
    const unsigned int counter_max = 10; // Default to 100ms period
#endif

    counter++;
    if (counter >= counter_max)
    {
        counter = 0;
        led_state = !led_state;
        digitalWrite(LED_BUILTIN, led_state ? HIGH : LOW);
    }
}

extern "C"
{
    void Task_Init(void)
    {
        // initialize LED digital pin as an output.
        pinMode(LED_BUILTIN, OUTPUT);
    }

    void Task_100ms(void)
    {
#if (defined(CONFIG_OS_TASK_PERIOD) && CONFIG_OS_TASK_PERIOD == 100)
        blink_led();
#endif
    }

    void Task_10ms(void)
    {
#if (defined(CONFIG_OS_TASK_PERIOD) && CONFIG_OS_TASK_PERIOD == 10)
        blink_led();
#endif
    }
}
