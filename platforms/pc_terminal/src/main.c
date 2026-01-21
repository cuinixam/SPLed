#include "autoconf.h"
#include "os.h"
#include <stdio.h>
#include <string.h>

static void print_help(void) {
  printf("SPLed PC Terminal\n");
  printf("\n");
  printf("Keyboard Commands:\n");
  printf("  Press 'P' to power ON. Press it again to power OFF.\n");
#if CONFIG_BLINKING
  printf("  Arrow Up/Down control the blinking frequency.\n");
#elif defined(CONFIG_BRIGHTNESS_ADJUSTMENT_MANUAL) &&                          \
    CONFIG_BRIGHTNESS_ADJUSTMENT_MANUAL == 1
  printf("  Arrow Up/Down control the brightness of the light.\n");
#elif defined(CONFIG_BRIGHTNESS_ADJUSTMENT_AUTOMATIC) &&                       \
    CONFIG_BRIGHTNESS_ADJUSTMENT_AUTOMATIC == 1
  printf("  Arrow Up/Down have no effect.\n");
#endif
}

int main(int argc, char *argv[]) {
  if (argc > 1 && strcmp(argv[1], "--help") == 0) {
    print_help();
    return 0;
  }

  OsRun();

  return 0;
}
