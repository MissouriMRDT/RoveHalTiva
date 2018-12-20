///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MRDT 2019 => TivaC1294/TivaC129E Launchpad Timer Interrupt Module (Timer ISR)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ROVE_TIMER_INTERRUPT_H
#define ROVE_TIMER_INTERRUPT_H

#include "RoveTimerNumbers.h"
#include "RoveTimerHardware.h"
#include "stdint.h"

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