///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MRDT 2019 => TivaC1294/TivaC129E Launchpad Timer Interrupt Module (Timer ISR)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ROVE_TIMER_HARDWARE_H
#define ROVE_TIMER_HARDWARE_H

#include "stdint.h"
#include "driverlib/timer.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Todo...Note: These two should be TivaWare functions... but were never implemented by the TivaWare DriverLib team => TimerValueGet returned a 16 bit timer, not the full 24 bit timer, TimerConfigure stomps on sthe other half AB timer
// Comme on TI wtf...

uint32_t TimerValueGet24(    uint32_t TIMER_BASE_ADDRESS, uint32_t TIMER_CHANNEL_AB );
void     TimerConfigure16AB( uint32_t TIMER_BASE_ADDRESS, uint32_t TIMER_CHANNEL_AB, uint32_t TIMER_CONFIGURE );


namespace roveware /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
  enum  timer_clock_source_t { TIMER_USE_SYSCLOCK=0, 
                               TIMER_USE_PIOSC=1 };                                 // timer_clock_source = 120Mhz SYSCLOCK or 16Mhz PIOSC

  static const int      SYSCLOCK_TICKS_PER_MICRO                = ( 120000000 / 1000000 ); // 120Mhz / ( micros / sec )
  static const int      PIOSC_TICKS_PER_MICRO                   = (  16000000 / 1000000 ); //  16Mhz / ( micros / sec )

  static const uint32_t TIMER_USE_PERIODIC_UP_AB                = ( TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC_UP | TIMER_CFG_B_PERIODIC_UP );
  static const uint32_t TIMER_USE_CAPTURE_TICKS_AB              = ( TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME    | TIMER_CFG_B_CAP_TIME    );

  struct TimerHardware //////////////////
  {
    volatile uint32_t TIMER_PERIPHERAL;
    volatile uint32_t TIMER_BASE_ADDRESS;
    volatile uint32_t TIMER_CHANNEL_AB;
    volatile uint32_t TIMER_INTERRUPT;
  };

  struct Timer /////////////////////////////
  {
    volatile uint32_t      interrupt_source;
    volatile uint32_t      period_ticks;
    struct   TimerHardware Hardware;

    void (*timerIsr)(     void );
    void (*userFunction)( void );
  };

  typedef void (*isrPtr)( void );

  void timerIsrPeriodic_0A ( void );
  void timerIsrPeriodic_0B ( void );
  void timerIsrPeriodic_1A ( void );
  void timerIsrPeriodic_1B ( void );
  void timerIsrPeriodic_2A ( void );
  void timerIsrPeriodic_2B ( void );
  void timerIsrPeriodic_3A ( void );
  void timerIsrPeriodic_3B ( void );
  void timerIsrPeriodic_4A ( void );
  void timerIsrPeriodic_4B ( void );
  void timerIsrPeriodic_5A ( void );
  void timerIsrPeriodic_5B ( void );
  void timerIsrPeriodic_6A ( void );
  void timerIsrPeriodic_6B ( void );
  void timerIsrPeriodic_7A ( void );
  void timerIsrPeriodic_7B ( void );

  bool                 isTimerValid(     int timer );
  void                 attachTimer(      int timer, struct Timer* Timer );
  void                 dispatchTimerIsrPeriodic(    struct Timer* Timer );
  int                  pinToTimer(       int pin ); 
  isrPtr               timerIsrPeriodic( int timer );
  struct TimerHardware timerHardware (   int timer );

  void configureTimer( uint32_t TIMER_CLOCK_SOURCE, uint32_t TIMER_CONFIGURE,  uint32_t TIMER_PERIPHERAL,       uint32_t TIMER_BASE_ADDRESS, uint32_t TIMER_CHANNEL_AB );
  void attachTimerIsr( uint32_t TIMER_BASE_ADDRESS, uint32_t TIMER_CHANNEL_AB, uint32_t TIMER_INTERRUPT_SOURCE, uint32_t TIMER_INTERRUPT, void(*timerIsr)(void), uint8_t priority );
  void startTimer(     uint32_t TIMER_BASE_ADDRESS, uint32_t TIMER_CHANNEL_AB, uint32_t TIMER_INTERRUPT_SOURCE, uint32_t TIMER_PERIOD_TICKS_24 );
  void stopTimer(      uint32_t TIMER_BASE_ADDRESS, uint32_t TIMER_INTERRUPT_SOURCE );

}// end namespace roveware ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // ROVE_TIMER_HARDWARE_H