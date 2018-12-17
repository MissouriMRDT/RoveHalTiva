///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MRDT 2019 => TivaC1294/TivaC129E Launchpad Timer Interrupt Module (Timer ISR)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ROVE_TIMER_INTERRUPT_H
#define ROVE_TIMER_INTERRUPT_H

#include "RoveTimerNumbers.h"
#include "RoveTimerHardware.h"
#include "stdint.h"

// T0_A     =>     PD_0, PA_0, PL_4     PIN_CONFLICTS     Energia::analogWrite or roveware::PwmRead
// T0_B     =>     PD_1, PA_1, PL_5     PIN_CONFLICTS     Energia::analogWrite or RoveWare::PwmRead
// T1_A     =>     PD_2, PA_2, PL_6     PIN_CONFLICTS     Energia::analogWrite or RoveWare::PwmRead 
// T1_B     =>     PD_3, PA_3, PL_7     PIN_CONFLICTS     Energia::analogWrite or RoveWare::PwmRead
// T2_A     =>     PA_4, PM_0           PIN_CONFLICTS     Energia::analogWrite or RoveWare::PwmRead or Energia::Servo
// T2_B     =>     PA_5, PM_1           PIN_CONFLICTS     Energia::analogWrite or RoveWare::PwmRead
// T3_A     =>     PA_6, PM_2, PD_4     PIN_CONFLICTS     Energia::analogWrite or RoveWare::PwmRead
// T3_B     =>     PA_7, PM_3, PD_5     PIN_CONFLICTS     Energia::analogWrite or RoveWare::PwmRead
// T4_A     =>     PM_4, PB_0, PD_6     PIN_CONFLICTS     Energia::analogWrite or RoveWare::PwmRead
// T4_B     =>     PM_5, PB_1, PD_7     PIN_CONFLICTS     Energia::analogWrite or RoveWare::PwmRead
// T5_A     =>     PM_6, PB_2           PIN_CONFLICTS     Energia::analogWrite or RoveWare::PwmRead
// T5_B     =>     PM_7, PB_3           PIN_CONFLICTS     Energia::analogWrite or RoveWare::PwmRead
// T6_A     =>                       No PIN_CONFLICTS 
// T6_B     =>                       No PIN_CONFLICTS
// T7_A     =>                       No PIN_CONFLICTS
// T7_B     =>                       No PIN_CONFLICTS

class RoveTimerInterrupt /////////////////////////////////////////////////////////////////////////////////////////////////////////
{
public: // => attach the userFunction to a 24 bit periodic halfwidth prescale (16b+8b) timer interrupt by timer_ab number (1~16)

  RoveTimerInterrupt( int timer );

  void attach(       void(*userFunction)(void), int period_millis, int priority=7 ); //, timer_clock_source_t clock_source=USE_PIOSC );
  void attachMicros( void(*userFunction)(void), int period_micros, int priority=7 ); //, timer_clock_source_t clock_source=USE_PIOSC );

  void stop();
  void start( );

private:

  uint8_t timer;
  struct  roveware::Timer Timer;
};

#endif // ROVE_TIMER_INTERRUPT_H