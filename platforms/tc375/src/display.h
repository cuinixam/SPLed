#ifndef DISPLAY_H
#define DISPLAY_H

/*
 * 4-digit 7-segment display on the Arduino multi-function shield.
 *
 * This is a platform-private module, not one of the product line's interfaces.
 * Nothing in components/ knows it exists. It reads the RTE directly and is
 * driven from the platform's own main loop, so no other platform has to grow a
 * stub for it.
 *
 * Layout, left to right:
 *
 *     [power state] [ knob value, right aligned ]
 *          0/1              0 .. 100
 */

/* Configure the shift-register pins and blank the display. Call once. */
void display_init(void);

/* Draw one full frame. Call once per cycle, after spled(). */
void display_update(void);

#endif /* DISPLAY_H */
