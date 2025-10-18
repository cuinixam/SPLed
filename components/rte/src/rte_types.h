/**
 * @file rte.h
 * @brief Runtime environment (RTE) interface for power and light functionalities.
 */

#ifndef RTE_TYPES_H
#define RTE_TYPES_H

#include "platform_types.h"

/**
 * @brief Enumerated type for the input keys.
 */
typedef enum
{
    KEY_UP = 0x26,          /**< Up arrow key. */
    KEY_DOWN = 0x28,        /**< Down arrow key. */
    POWER_BUTTON_KEY = 'P', /**< Power button key. */
} KeyCodes;

/**
 * @brief Enumerated type for power states.
 */
typedef enum
{
    POWER_STATE_OFF = 0, /**< Power is turned off. */
    POWER_STATE_ON = 1   /**< Power is turned on. */
} PowerState;

/**
 * @brief Struct representing an RGB color.
 */

typedef struct
{
    uint8_t red;   /**< The red component of the color. */
    uint8_t green; /**< The green component of the color. */
    uint8_t blue;  /**< The blue component of the color. */
} RGBColor;

/**
 * @typedef Percentage
 * @brief Represents a positive percentage value ranging from 0 to 100.
 */
typedef uint8_t percentage_t;

/**
 * @typedef Brightness
 * @brief Represents a brightness value ranging from 0 to 255.
 */
typedef unsigned int brightness_t;

#endif // RTE_TYPES_H
