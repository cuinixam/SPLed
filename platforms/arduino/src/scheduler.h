#include "platform_types.h"
/* How many different functions will be scheduled */
#define executionListLength 10
#define schedulerNoCallable NULL

#define CFG_USED_TIMER 1

class Scheduler
{
public:
   enum Period
   {
      INIT = 0,
      TASK_10MS = 10,
      TASK_100MS = 100
   };

private:
   typedef void (*callable)();
   Scheduler::callable init;
   Scheduler::callable runnable_10ms;
   Scheduler::callable runnable_100ms;
   unsigned char counter_1ms = 0;
   unsigned char counter_10ms = 0;

public:
   Scheduler();
   void Attach(Scheduler::callable runnable, Period period);
   void Start(void);
   void Task_10ms(void);
   void Task_1ms(void);

private:
   void ConfigureTimer(void);
   void ConfigureTimer1(void);
   void ConfigureTimer2(void);
   void StartTimer(void);
};

extern Scheduler scheduler;
