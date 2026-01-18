#include "led_interface.h"

#include "rte.h"
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

static RGBColor previousLightValue = {0, 0, 0};

void ledInterface_init(void)
{
    previousLightValue.red = 0;
    previousLightValue.green = 0;
    previousLightValue.blue = 0;

#ifdef _WIN32
    // Get the handle to the current output buffer
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Hide the cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
#else
    // Hide cursor on Unix/Linux using ANSI escape codes
    printf("\x1b[?25l");
#endif

    // Display LED with black background initially (power off state)
    printf("\x1b[48;2;0;0;0m"); // Black background
    printf("LED\r");
    fflush(stdout);
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

        // Print the LED representation with the new background color
        printf("\x1b[48;2;%d;%d;%dm", lightValue.red, lightValue.green, lightValue.blue);
        printf("LED\r");
        fflush(stdout);
    }
}
