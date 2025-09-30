#ifndef mcu_h_
#define mcu_h_

#include <Arduino.h>
#include "hw_config.h"
#include "platform_types.h"

#define EnableInterrupts() sei()
#define DisableInterrupts() cli()

#endif
