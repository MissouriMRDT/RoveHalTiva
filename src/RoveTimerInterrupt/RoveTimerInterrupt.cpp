///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MRDT 2019 => Tiva C 1294/129E Launchpad Timer Interrupt Module (Timer ISR)
// Todo      => add support for the low frequency oscillator as a clock source (presently only supports the system clock source or the precision oscillator clock source)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "RoveTimerInterrupt.h"
#include "RoveTimerNumbers.h"
#include "RoveTimerHardware.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_nvic.h"

#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"

#include "tm4c1294ncpdt.h"

#include "stdint.h"

RoveTimerInterrupt::RoveTimerInterrupt( int timer ) ///////////////////////////////////
{
  this->timer           = timer;
  if ( ( this->timer %  2) == 0 ){ this->Timer.interrupt_source = TIMER_TIMB_TIMEOUT; }
  else {                           this->Timer.interrupt_source = TIMER_TIMA_TIMEOUT; }

  this->Timer.Hardware  = roveware::timerHardware(    timer );
  this->Timer.timerIsr  = roveware::timerIsrPeriodic( timer );
  roveware::attachTimer( this->timer, &(this->Timer) );
}

void RoveTimerInterrupt::attach( void(*userFunction)(void), int period_millis, int priority ) //
{                  this->attachMicros( userFunction,     1000 * period_millis,     priority ); }

void RoveTimerInterrupt::attachMicros( void(*userFunction)(void), int period_micros, int priority ) /////////////////////////////////////////////
{
  if ( roveware::isTimerValid( this->timer ) ) 
  { 
    this->Timer.period_ticks = period_micros * roveware::PIOSC_TICKS_PER_MICRO;
    this->Timer.userFunction  = userFunction;

    roveware::configureTimer( roveware::TIMER_USE_PIOSC,               roveware::TIMER_USE_PERIODIC_UP_AB,
                              this->Timer.Hardware.TIMER_PERIPHERAL,   this->Timer.Hardware.TIMER_BASE_ADDRESS );
    roveware::attachTimerIsr( this->Timer.Hardware.TIMER_BASE_ADDRESS, this->Timer.Hardware.TIMER_CHANNEL_AB, 
                              this->Timer.interrupt_source,            this->Timer.Hardware.TIMER_INTERRUPT, this->Timer.timerIsr, priority );
  }
}

void RoveTimerInterrupt::start() ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{  if ( roveware::isTimerValid( this->timer ) ) 
  {     roveware::startTimer(   this->Timer.Hardware.TIMER_BASE_ADDRESS, this->Timer.Hardware.TIMER_CHANNEL_AB, this->Timer.interrupt_source, this->Timer.period_ticks ); } 
}

void RoveTimerInterrupt::stop() ///////////////////////////////////////////////////////////////////////////////
{  if ( roveware::isTimerValid( this->timer ) ) 
  {     roveware::stopTimer(    this->Timer.Hardware.TIMER_BASE_ADDRESS, this->Timer.interrupt_source ); }
}
