#include "autoconf.h"
#include "spled.h"
#include "led_interface.h"
#include "button_interface.h"

#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "Bsp.h"

IFX_ALIGN(4) IfxCpu_syncEvent g_cpuSyncEvent = 0;

static void Hal_Init(void)
{
    ledInterface_init();
    buttonInterface_init();
}

/* Entry point for CPU0. Infineon's startup code (Ifx_Ssw_Tc0.c) calls this, so
   the name is fixed and there is no main(). */
void core0_main(void)
{
    IfxCpu_enableInterrupts();

    /* The CPU and safety watchdogs are enabled out of reset and would reset the
       device if left unserviced. Nothing here services them, so both go off.
       This is separate from the board's TLF35584 watchdog, which jumper JP601
       disables in hardware. */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    Hal_Init();

    while (1)
    {
        buttonInterface_update();
        spled();
        waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, CONFIG_OS_TASK_PERIOD));
    }
}

/* CPU1 and CPU2 are unused, but the linker script defines a start address for
   each core, so both entry points have to exist. They park after signalling the
   sync event CPU0 waits on. */
void core1_main(void)
{
    IfxCpu_enableInterrupts();
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);
    while (1)
    {
    }
}

void core2_main(void)
{
    IfxCpu_enableInterrupts();
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);
    while (1)
    {
    }
}

/* Ifx_Ssw_doCppInit() calls _init(), which normally comes from crti.o. The link
   uses -nostartfiles, and there are no C++ static constructors, so an empty
   definition is the whole implementation. */
void _init(void)
{
}
