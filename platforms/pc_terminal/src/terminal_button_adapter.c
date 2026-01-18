#include "button_interface.h"
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>


static struct termios orig_termios;
static int kbhit_initialized = 0;
static int stdin_flags;

// State validation variables
// Counters to keep keys "pressed" for multiple frames (simulating hold for
// debounce)
#define KEY_HOLD_FRAMES 15 // Ensure it covers POWER_BUTTON_PRESS_DEBOUNCE (10)

static uint8_t key_state[256];
static uint8_t arrow_up_pressed = 0;
static uint8_t arrow_down_pressed = 0;

// Set terminal to non-canonical mode for key detection
static void enable_raw_mode(void) {
  struct termios raw;
  if (kbhit_initialized)
    return;

  // Check if stdin is a TTY
  if (!isatty(STDIN_FILENO))
    return;

  // Save original terminal settings
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    return;

  raw = orig_termios;

  // Disable canonical mode, echo, and related flags
  // Keep ISIG enabled so Ctrl+C (SIGINT) still works
  raw.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON | IEXTEN);

  // Disable input processing
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

  // Keep output processing enabled for ANSI escape codes
  // raw.c_oflag &= ~(OPOST); // DON'T disable this

  // Set character size to 8 bits
  raw.c_cflag &= ~(CSIZE | PARENB);
  raw.c_cflag |= CS8;

  // Non-blocking read
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 0;

  // Apply terminal settings with TCSAFLUSH to discard pending input
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    return;

  // Drain any remaining input in the buffer
  char discard[256];
  while (read(STDIN_FILENO, discard, sizeof(discard)) > 0)
    ; // Keep draining

  // Set stdin to non-blocking
  stdin_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, stdin_flags | O_NONBLOCK);

  kbhit_initialized = 1;
}

// Restore terminal to original mode
static void disable_raw_mode(void) {
  if (kbhit_initialized) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    fcntl(STDIN_FILENO, F_SETFL, stdin_flags);
    kbhit_initialized = 0;
  }
}

/**
 * Simulation Artifact: This function exists ONLY because we are simulating
 * hardware on a PC terminal. On real hardware (Arduino), buttons would have
 * physical latching behavior. This simulates that by keeping keys "pressed"
 * for multiple frames to satisfy debounce logic in higher-level components.
 */
static void simulate_hardware_behavior(void) {
  // Decay state for this frame (simulate key release after hold time)
  for (int k = 0; k < 256; k++) {
    if (key_state[k] > 0)
      key_state[k]--;
  }
  if (arrow_up_pressed > 0)
    arrow_up_pressed--;
  if (arrow_down_pressed > 0)
    arrow_down_pressed--;
}

static void update_keys_linux(void) {
  simulate_hardware_behavior();

  unsigned char buf[64];
  ssize_t n, i;

  // Read all available input
  while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
    for (i = 0; i < n; i++) {
      if (buf[i] == 27) // Escape sequence start
      {
        if (i + 2 < n && buf[i + 1] == '[') {
          if (buf[i + 2] == 'A')
            arrow_up_pressed = KEY_HOLD_FRAMES;
          else if (buf[i + 2] == 'B')
            arrow_down_pressed = KEY_HOLD_FRAMES;
          i += 2; // Skip sequence
        }
      } else {
        key_state[buf[i]] = KEY_HOLD_FRAMES;
      }
    }
  }
}
#endif

void buttonInterface_init(void) {
#ifdef _WIN32
  // No initialization needed for Windows console
#else
  enable_raw_mode();
  atexit(disable_raw_mode);
#endif
}

void buttonInterface_update(void) {
#ifdef _WIN32
  // Windows GetAsyncKeyState checks hardware state directly, no update step
  // needed
#else
  update_keys_linux();
#endif
}

boolean ButtonInterfaceIsButtonPressed(KeyCodes key) {
#ifdef _WIN32
  return (GetAsyncKeyState(key) & 0x8000) != 0;
#else
  if (key == KEY_UP)
    return arrow_up_pressed;
  if (key == KEY_DOWN)
    return arrow_down_pressed;

  // Check for lower or upper case
  char k = (char)key;
  if (key_state[(unsigned char)k])
    return TRUE;

  // Simple case conversion check
  if (k >= 'a' && k <= 'z') {
    if (key_state[(unsigned char)(k - 32)])
      return TRUE;
  } else if (k >= 'A' && k <= 'Z') {
    if (key_state[(unsigned char)(k + 32)])
      return TRUE;
  }

  return FALSE;
#endif
}
