#include "scheduler.h"
#include "mcu.h"

Scheduler scheduler;

Scheduler::Scheduler()
{
   counter_1ms = 0;
   counter_10ms = 0;
   runnable_10ms = schedulerNoCallable;
   runnable_100ms = schedulerNoCallable;
}

void Scheduler::Attach(Scheduler::callable runnable, Period period)
{
   switch (period)
   {
   case TASK_10MS:
      runnable_10ms = runnable;
      break;
   case TASK_100MS:
      runnable_100ms = runnable;
      break;
   }
}

void Scheduler::ConfigureTimer1(void)
{
   //set timer1 interrupt at 100Hz
   TCCR1A = 0; // set entire TCCR1A register to 0
   TCCR1B = 0; // same for TCCR1B
   TCNT1 = 0;  //initialize counter value to 0
   // set compare match register for 1hz increments
   OCR1A = 156;
   // turn on CTC mode
   TCCR1B |= (1 << WGM12);
   // enable timer compare interrupt
   TIMSK1 = (1 << OCIE1A);
   // TImer/counter disabled
   TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
}

void Scheduler::ConfigureTimer2(void)
{
   //set timer2 interrupt at 1000Hz
   TCCR2A = 0; // set entire TCCR2A register to 0
   TCCR2B = 0; // same for TCCR2B
   TCNT2 = 0;
   // set compare match register for 100hz increments
   OCR2A = 249;
   // turn on CTC mode
   TCCR2A |= (1 << WGM21);
   // enable timer compare interrupt
   TIMSK2 |= (1 << OCIE2A);
   // Timer/counter disabled
   TCCR2B &= ~(_BV(CS20) | _BV(CS21) | _BV(CS22));
}

void Scheduler::ConfigureTimer(void)
{
   DisableInterrupts();
#if (CFG_USED_TIMER == 1)
   ConfigureTimer1();
#else
   ConfigureTimer2();
#endif
   EnableInterrupts();
}

void Scheduler::StartTimer(void)
{
#if (CFG_USED_TIMER == 1)
   // Enable 1024 prescaler
   TCCR1B |= (_BV(CS12) | _BV(CS10));
#else
   // Enable 64 prescaler
   TCCR2B |= _BV(CS22);
#endif
}

void Scheduler::Start(void)
{
   ConfigureTimer();
   StartTimer();
}

void Scheduler::Task_10ms(void)
{
   counter_10ms++;
   if (runnable_10ms) runnable_10ms();
   if (counter_10ms >= 10)
   {
      counter_10ms = 0;
      if (runnable_100ms) runnable_100ms();
   }
}

void Scheduler::Task_1ms(void)
{
   counter_1ms++;
   if (counter_1ms >= 10)
   {
      counter_1ms = 0;
      Task_10ms();
   }
}

#if (CFG_USED_TIMER == 1)
/* Timer1 interrupt */
ISR(TIMER1_COMPA_vect)
{
   scheduler.Task_10ms();
}
#else
/* Timer2 interrupt */
ISR(TIMER2_COMPA_vect)
{
   scheduler.Task_1ms();
}
#endif
