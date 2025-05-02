/**
 * @file
 */

#include <gtest/gtest.h>
using namespace testing;

extern "C"
{
#include "power_button.h"
#include "rte.h"
}

#include "mockup_components_power_button.h"

/*!
 * @rst
 *
 * .. test:: power_button.test_key_press_and_release
 *    :id: TS_KI-001
 *    :tests: SWDD_KI-001, SWDD_KI-002, SWDD_KI-003
 *
 * @endrst
 */
TEST(power_button, test_key_press_and_release)
{
    CREATE_MOCK(mymock);

    // Key is initially not pressed
    EXPECT_CALL(mymock, RteIsKeyPressed(POWER_BUTTON_KEY)).WillOnce(Return(FALSE));
    EXPECT_CALL(mymock, RteSetPowerKeyPressedEvent(FALSE)).Times(1);
    powerButton();

    // Key is pressed but not yet debounced
    for (unsigned int i = 0; i < POWER_BUTTON_PRESS_DEBOUNCE - 1; i++)
    {
        EXPECT_CALL(mymock, RteIsKeyPressed(POWER_BUTTON_KEY)).WillOnce(Return(TRUE));
        EXPECT_CALL(mymock, RteSetPowerKeyPressedEvent(FALSE)).Times(1);
        powerButton();
    }

    // Key is pressed and debounced
    EXPECT_CALL(mymock, RteIsKeyPressed(POWER_BUTTON_KEY)).WillOnce(Return(TRUE));
    EXPECT_CALL(mymock, RteSetPowerKeyPressedEvent(TRUE)).Times(1);
    powerButton();

    // Key remains pressed, but the function should not be triggered again until debounce
    for (unsigned int i = 0; i < 2 * POWER_BUTTON_PRESS_DEBOUNCE; i++)
    {
        EXPECT_CALL(mymock, RteIsKeyPressed(POWER_BUTTON_KEY)).WillOnce(Return(TRUE));
        EXPECT_CALL(mymock, RteSetPowerKeyPressedEvent(FALSE)).Times(1);
        powerButton();
    }

    // Key is released but not yet debounced
    for (unsigned int i = 0; i < POWER_BUTTON_RELEASE_DEBOUNCE - 1; i++)
    {
        EXPECT_CALL(mymock, RteIsKeyPressed(POWER_BUTTON_KEY)).WillOnce(Return(FALSE));
        EXPECT_CALL(mymock, RteSetPowerKeyPressedEvent(FALSE)).Times(1);
        powerButton();
    }

    // Key is released and debounced
    EXPECT_CALL(mymock, RteIsKeyPressed(POWER_BUTTON_KEY)).WillOnce(Return(FALSE));
    EXPECT_CALL(mymock, RteSetPowerKeyPressedEvent(FALSE)).Times(1);
    powerButton();

    // Key remains released, but the function should not be triggered again until debounce
    for (unsigned int i = 0; i < 2 * POWER_BUTTON_RELEASE_DEBOUNCE; i++)
    {
        EXPECT_CALL(mymock, RteIsKeyPressed(POWER_BUTTON_KEY)).WillOnce(Return(FALSE));
        EXPECT_CALL(mymock, RteSetPowerKeyPressedEvent(FALSE)).Times(1);
        powerButton();
    }

    // Key is pressed again but not yet debounced
    for (unsigned int i = 0; i < POWER_BUTTON_PRESS_DEBOUNCE - 1; i++)
    {
        EXPECT_CALL(mymock, RteIsKeyPressed(POWER_BUTTON_KEY)).WillOnce(Return(TRUE));
        EXPECT_CALL(mymock, RteSetPowerKeyPressedEvent(FALSE)).Times(1);
        powerButton();
    }

    // Key is pressed and debounced
    EXPECT_CALL(mymock, RteIsKeyPressed(POWER_BUTTON_KEY)).WillOnce(Return(TRUE));
    EXPECT_CALL(mymock, RteSetPowerKeyPressedEvent(TRUE)).Times(1);
    powerButton();
}
