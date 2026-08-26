#ifndef SPLED_API_H
#define SPLED_API_H

#include <stdint.h>

#ifdef _WIN32
#define SPLED_API __declspec(dllexport)
#else
#define SPLED_API
#endif

// Run the variant's start-up once.
SPLED_API void spled_init(void);

// Advance the variant by one task period.
SPLED_API void spled_step(void);

// Current LED colour, as written through led_interface.
SPLED_API void spled_get_led(uint8_t *red, uint8_t *green, uint8_t *blue);

// Press or release one button, using the codes from KeyCodes.
SPLED_API void spled_set_button(int key_code, int pressed);

// The task period the variant was configured with, in milliseconds.
SPLED_API int spled_task_period_ms(void);

// Internal state, read straight from the RTE, for the debug panel in gui.py.

// Current power state, using the values from PowerState.
SPLED_API int spled_get_power_state(void);

// Current main knob value, as a percentage.
SPLED_API int spled_get_main_knob_value(void);

// Current brightness. Only meaningful when the variant selected brightness adjustment;
// without it light_controller uses a fixed value and never writes this one.
SPLED_API int spled_get_brightness(void);

#endif /* SPLED_API_H */
