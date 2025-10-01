#include "mcu.h"
#include "scheduler.h"

extern "C"
{
#include "rte.h"
}

void setup()
{
   scheduler.Attach(Task_Init, Scheduler::INIT);
   scheduler.Attach(Task_100ms, Scheduler::TASK_100MS);
   scheduler.Attach(Task_10ms, Scheduler::TASK_10MS);
   scheduler.Start();
}

void loop()
{
}
