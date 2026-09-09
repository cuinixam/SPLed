#include "display.h"
#include "hw_config.h"
#include "rte.h"

#include "Bsp.h"
#include "IfxPort.h"
#include "IfxStm.h"

/*
 * The shield's display is two 74HC595 shift registers in cascade driving a
 * 4-digit common-anode display. One register holds the segment pattern, the
 * other selects which digit is lit. Only one digit is ever lit at a time, so the
 * frame has to be redrawn continuously or it goes dark.
 *
 * Common anode means the segment byte is active low: a 0 bit lights a segment,
 * and 0xFF is a blank digit.
 *
 * Byte order, established on the board: the segment byte is shifted out FIRST and
 * the digit byte LAST. The two registers are cascaded, so the first byte pushed
 * travels through to the far register and the last one stays in the near one.
 * Sending them the other way round parks 0x00 in the segment register during
 * blanking, which lights every segment of every digit - the display sits fully
 * lit and never shows a value.
 *
 * Still unconfirmed: DIGIT_SELECT is written leftmost digit first. If the value
 * reads back to front, reverse that array.
 */
#define DISPLAY_SHIFT_DIGIT_FIRST 0

#define DISPLAY_DIGITS 4u

/* How long each digit stays lit. Four digits at 1 ms gives a 250 Hz frame rate
 * with no visible flicker, and costs 4 ms of the cycle. See display_update(). */
#define DISPLAY_DIGIT_ON_MS 1u

/* Common-anode segment patterns, bit 0 = segment a .. bit 6 = g, bit 7 = dp. */
static const uint8 DIGIT_GLYPH[10] = {
    0xC0u, /* 0 */
    0xF9u, /* 1 */
    0xA4u, /* 2 */
    0xB0u, /* 3 */
    0x99u, /* 4 */
    0x92u, /* 5 */
    0x82u, /* 6 */
    0xF8u, /* 7 */
    0x80u, /* 8 */
    0x90u, /* 9 */
};

#define GLYPH_BLANK 0xFFu

/* Digit enable bytes, leftmost digit first. */
static const uint8 DIGIT_SELECT[DISPLAY_DIGITS] = {0xF1u, 0xF2u, 0xF4u, 0xF8u};

static void shiftOutByte(uint8 value)
{
    uint8 bit;

    /* 74HC595 samples on the rising clock edge, most significant bit first. */
    for (bit = 0u; bit < 8u; bit++)
    {
        if ((value & 0x80u) != 0u)
        {
            IfxPort_setPinHigh(SEG_DATA_PORT, SEG_DATA_PIN);
        }
        else
        {
            IfxPort_setPinLow(SEG_DATA_PORT, SEG_DATA_PIN);
        }
        value = (uint8)(value << 1u);

        IfxPort_setPinHigh(SEG_CLK_PORT, SEG_CLK_PIN);
        IfxPort_setPinLow(SEG_CLK_PORT, SEG_CLK_PIN);
    }
}

static void writeDigit(uint8 select, uint8 glyph)
{
    /* Hold the latch low while shifting so the outputs do not flicker through
       intermediate states, then raise it to publish all 16 bits at once. */
    IfxPort_setPinLow(SEG_LATCH_PORT, SEG_LATCH_PIN);

#if DISPLAY_SHIFT_DIGIT_FIRST
    shiftOutByte(select);
    shiftOutByte(glyph);
#else
    shiftOutByte(glyph);
    shiftOutByte(select);
#endif

    IfxPort_setPinHigh(SEG_LATCH_PORT, SEG_LATCH_PIN);
}

static void blankDisplay(void)
{
    /* 0x00 selects no digit at all, so nothing is lit whatever the segments say. */
    writeDigit(0x00u, GLYPH_BLANK);
}

/*
 * Left digit is the power state. The remaining three are the knob value, right
 * aligned with leading zeros blanked, so 5 reads as "   5" and 100 as " 100".
 */
static void buildFrame(uint8 frame[DISPLAY_DIGITS])
{
    const PowerState power = RteGetPowerState();
    const percentage_t knob = RteGetMainKnobValue();
    const uint8 hundreds = (uint8)(knob / 100u);
    const uint8 tens = (uint8)((knob / 10u) % 10u);
    const uint8 ones = (uint8)(knob % 10u);

    frame[0] = DIGIT_GLYPH[(power == POWER_STATE_ON) ? 1u : 0u];
    frame[1] = (hundreds > 0u) ? DIGIT_GLYPH[hundreds] : GLYPH_BLANK;
    frame[2] = ((hundreds > 0u) || (tens > 0u)) ? DIGIT_GLYPH[tens] : GLYPH_BLANK;
    frame[3] = DIGIT_GLYPH[ones];
}

void display_init(void)
{
    IfxPort_setPinModeOutput(SEG_LATCH_PORT, SEG_LATCH_PIN, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinModeOutput(SEG_CLK_PORT, SEG_CLK_PIN, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinModeOutput(SEG_DATA_PORT, SEG_DATA_PIN, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

    /* Idle state: clock low so the first rising edge is a real one, latch high. */
    IfxPort_setPinLow(SEG_CLK_PORT, SEG_CLK_PIN);
    IfxPort_setPinLow(SEG_DATA_PORT, SEG_DATA_PIN);
    IfxPort_setPinHigh(SEG_LATCH_PORT, SEG_LATCH_PIN);

    blankDisplay();
}

void display_update(void)
{
    uint8 frame[DISPLAY_DIGITS];
    uint8 digit;

    buildFrame(frame);

    for (digit = 0u; digit < DISPLAY_DIGITS; digit++)
    {
        writeDigit(DIGIT_SELECT[digit], frame[digit]);
        waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, DISPLAY_DIGIT_ON_MS));
    }

    /* Blank before returning. Without this the last digit stays lit for the whole
       task period and reads noticeably brighter than the other three. */
    blankDisplay();
}
