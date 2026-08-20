#ifndef hw_config_h
#define hw_config_h

/*
 * Pin map for a classic Arduino multi-function shield seated on the
 * KIT-A2G-TC375-ARD-SB (Hitex ShieldBuddy).
 *
 * The board is Arduino-form-factor but the software is not: the shield is wiring,
 * so the Arduino pin numbers below are only there to make the mapping checkable
 * against the ShieldBuddy user manual (doc 4269.40100, Table 5).
 *
 *   shield        Arduino   TC375
 *   LED1          D13       P10.2   (also the board's own user LED)
 *   LED2          D12       P10.1
 *   LED3          D11       P10.3
 *   button S1     A1        P32.4
 *   button S2     A2        P23.1
 *   button S3     A3        P33.9
 *
 * The board is 5V-only (UM 3.1), which is what a 5V multi-function shield wants,
 * so no level shifting is involved. Two harmless overlaps: the LED pins are also
 * the QSPI0 bus, and P33.9 is also ASC2 TX. Neither peripheral is used here.
 */

#define LED_RED_PORT    &MODULE_P10
#define LED_RED_PIN     2
#define LED_GREEN_PORT  &MODULE_P10
#define LED_GREEN_PIN   1
#define LED_BLUE_PORT   &MODULE_P10
#define LED_BLUE_PIN    3

#define BUTTON_UP_PORT     &MODULE_P32
#define BUTTON_UP_PIN      4
#define BUTTON_DOWN_PORT   &MODULE_P23
#define BUTTON_DOWN_PIN    1
#define BUTTON_POWER_PORT  &MODULE_P33
#define BUTTON_POWER_PIN   9

#endif // hw_config_h
