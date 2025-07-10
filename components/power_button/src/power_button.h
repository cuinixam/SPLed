#ifndef POWER_BUTTON_H
#define POWER_BUTTON_H

#define POWER_BUTTON_KEY 'P'

#define POWER_BUTTON_PRESS_DEBOUNCE 10
#define POWER_BUTTON_RELEASE_DEBOUNCE 10

void powerButtonInit(void);
void powerButton(void);

#endif /* POWER_BUTTON_H */
