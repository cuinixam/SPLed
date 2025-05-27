/**
 * @file brightness_controller.c
 * @brief Module to control the brightness.
 */

#include "brightness_controller.h"
#include "rte.h"

#ifdef CONFIG_BRIGHTNESS_ADJUSTMENT_AUTOMATIC

#define BRIGHTNESS_PERIOD_TICKS ((unsigned int)((CONFIG_BRIGHTNESS_ADJUSTMENT_PERIOD * 1000) / BRIGHTNESS_TASK_PERIOD))

/**
 * @rst
 * .. impl:: Periodic Brightness Adjustment
 *    :id: SWIMPL_BC-001
 *    :implements: SWDD_BC-100, SWDD_BC-102
 * @endrst
 */
SPLE_TESTABLE_STATIC brightness_t periodicBrightnessAdjustment(BrightnessAdjustmentData *data)
{
    brightness_t brightnessValue = 0;

    if (data->ticksCounter < data->halfPeriod)
    {
        // Ramp up linearly from min to max
        brightnessValue = data->minBrightness + ((data->maxBrightness - data->minBrightness) * data->ticksCounter) / data->halfPeriod;
    }
    else
    {
        // Ramp down linearly from max to min
        brightnessValue = data->maxBrightness - ((data->maxBrightness - data->minBrightness) * (data->ticksCounter - data->halfPeriod)) / data->halfPeriod;
    }

    // Increment and reset ticksCounter based on the period
    data->ticksCounter = (data->ticksCounter + 1) % data->period;

    return brightnessValue;
}

#else /* CONFIG_BRIGHTNESS_ADJUSTMENT_AUTOMATIC */

/**
 * @rst
 * .. impl:: Manual Brightness Adjustment
 *    :id: SWIMPL_BC-002
 *    :implements: SWDD_BC-100, SWDD_BC-101, SWDD_BC-201
 * @endrst
 */
static brightness_t manualBrightnessAdjustment(void)
{
    percentage_t mainKnobValue = RteGetMainKnobValue();
    brightness_t brightnessValue = 0;

    if (mainKnobValue == 0)
    {
        brightnessValue = 0;
    }
    else
    {
        brightnessValue = (mainKnobValue * 255) / 100;
    }
    return brightnessValue;
}

#endif /* CONFIG_BRIGHTNESS_ADJUSTMENT_AUTOMATIC */

/**
 * @rst
 * .. impl:: Brightness Controller runnable
 *    :id: SWIMPL_BC-003
 *    :implements: SWDD_BC-200, SWDD_BC-202, SWDD_BC-203
 * @endrst
 */
void brightnessController(void)
{
    brightness_t brightnessValue = 0;
#ifdef CONFIG_BRIGHTNESS_ADJUSTMENT_AUTOMATIC
    static BrightnessAdjustmentData data = {
        .ticksCounter = 0,
        .period = BRIGHTNESS_PERIOD_TICKS,
        .halfPeriod = BRIGHTNESS_PERIOD_TICKS / 2,
        .maxBrightness = 200,
        .minBrightness = 50};

    brightnessValue = periodicBrightnessAdjustment(&data);
    RteSetBrightnessAdjustmentCounter(data.ticksCounter);
#elif defined(CONFIG_BRIGHTNESS_ADJUSTMENT_MANUAL)
    brightnessValue = manualBrightnessAdjustment();
#endif
    RteSetBrightnessValue(brightnessValue);
}
