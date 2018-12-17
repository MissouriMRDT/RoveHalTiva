///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MRDT 2019 => TivaC1294/TivaC129E Launchpad Timer Interrupt Module (Timer ISR)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 'TxCCPx'                              => Tiva C Series TM4C1294NCPDT Microcontroller Data Sheet (Rev. B) defines the module top down register hierarchy
//                                       => ti.com/lit/ds/symlink/tm4c1294ncpdt.pdf

// SYSCTL_PERIPH_TIMER0, TIMER_A, TIMER_TIMA_TIMEOUT => TivaWare Peripheral Driver Library provides todo
//                                                   => ti.com/lit/ug/spmu298d/spmu298d.pdf

// TIMER0_BASE, INT_TIMER0A,             => TivaWare Include todo provides todo
//                                       => tivaware/inc/

#include "RoveTimerHardware.h"
#include "RoveTimerNumbers.h"
#include "RoveTimerInterrupt.h"

#include "Energia.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_nvic.h"
#include "inc/hw_timer.h"

#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"

#include "tm4c1294ncpdt.h"

#include "stdint.h"

namespace roveware /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
  struct Timer* Timers[16] = {};

  void timerIsrPeriodic_0A ( void ) { dispatchTimerIsrPeriodic( Timers[ T0_A ] ); }
  void timerIsrPeriodic_0B ( void ) { dispatchTimerIsrPeriodic( Timers[ T0_B ] ); }
  void timerIsrPeriodic_1A ( void ) { dispatchTimerIsrPeriodic( Timers[ T1_A ] ); }
  void timerIsrPeriodic_1B ( void ) { dispatchTimerIsrPeriodic( Timers[ T1_B ] ); }
  void timerIsrPeriodic_2A ( void ) { dispatchTimerIsrPeriodic( Timers[ T2_A ] ); }
  void timerIsrPeriodic_2B ( void ) { dispatchTimerIsrPeriodic( Timers[ T2_B ] ); }
  void timerIsrPeriodic_3A ( void ) { dispatchTimerIsrPeriodic( Timers[ T3_A ] ); }
  void timerIsrPeriodic_3B ( void ) { dispatchTimerIsrPeriodic( Timers[ T3_B ] ); }
  void timerIsrPeriodic_4A ( void ) { dispatchTimerIsrPeriodic( Timers[ T4_A ] ); }
  void timerIsrPeriodic_4B ( void ) { dispatchTimerIsrPeriodic( Timers[ T4_B ] ); }
  void timerIsrPeriodic_5A ( void ) { dispatchTimerIsrPeriodic( Timers[ T5_A ] ); }
  void timerIsrPeriodic_5B ( void ) { dispatchTimerIsrPeriodic( Timers[ T5_B ] ); }
  void timerIsrPeriodic_6A ( void ) { dispatchTimerIsrPeriodic( Timers[ T6_A ] ); }
  void timerIsrPeriodic_6B ( void ) { dispatchTimerIsrPeriodic( Timers[ T6_B ] ); }
  void timerIsrPeriodic_7A ( void ) { dispatchTimerIsrPeriodic( Timers[ T7_A ] ); }
  void timerIsrPeriodic_7B ( void ) { dispatchTimerIsrPeriodic( Timers[ T7_B ] ); }

  bool isTimerValid( int timer ) 
  {          return (    timer >  INVALID ) and ( timer <= MAX_TIMER); }

  void attachTimer( int timer, struct Timer* Timer ) 
  {            Timers [ timer ]     = Timer; }

  void dispatchTimerIsrPeriodic( struct Timer* Timer )////////////////////////////
  {
    TimerIntClear(   Timer->Hardware.TIMER_BASE_ADDRESS, Timer->interrupt_source );
    TimerIntDisable( Timer->Hardware.TIMER_BASE_ADDRESS, Timer->interrupt_source );
    
    Timer->userFunction();
    
    TimerIntClear(  Timer->Hardware.TIMER_BASE_ADDRESS, Timer->interrupt_source );
    TimerIntEnable( Timer->Hardware.TIMER_BASE_ADDRESS, Timer->interrupt_source );
  }

 void configureTimer( uint32_t TIMER_CLOCK_SOURCE,
                      uint32_t TIMER_CONFIGURE,
                      uint32_t TIMER_PERIPHERAL,
                      uint32_t TIMER_BASE_ADDRESS )
  {
            SysCtlPeripheralEnable( TIMER_PERIPHERAL );
    while( !SysCtlPeripheralReady(  TIMER_PERIPHERAL ) )
    {   }; 
    TimerClockSourceSet( TIMER_BASE_ADDRESS, TIMER_CLOCK_SOURCE );
    TimerConfigure(      TIMER_BASE_ADDRESS, TIMER_CONFIGURE );
  }

  void attachTimerIsr( uint32_t TIMER_BASE_ADDRESS,
                       uint32_t TIMER_CHANNEL_AB,
                       uint32_t TIMER_INTERRUPT_SOURCE,
                       uint32_t TIMER_INTERRUPT,
                       void(*timerIsr)(void),
                       uint8_t priority )
  {
    TimerIntRegister( TIMER_BASE_ADDRESS, TIMER_CHANNEL_AB, timerIsr );
    TimerEnable(      TIMER_BASE_ADDRESS, TIMER_CHANNEL_AB );
    TimerIntEnable(   TIMER_BASE_ADDRESS, TIMER_INTERRUPT_SOURCE);
    TimerIntClear(    TIMER_BASE_ADDRESS, TIMER_INTERRUPT_SOURCE); 
    IntPrioritySet(   TIMER_INTERRUPT,  ( priority << 5 ));
    IntEnable(        TIMER_INTERRUPT );
    IntMasterEnable(); 
  }

  void startTimer( uint32_t TIMER_BASE_ADDRESS, 
                   uint32_t TIMER_CHANNEL_AB, 
                   uint32_t TIMER_INTERRUPT_SOURCE, 
                   uint32_t TIMER_PERIOD_TICKS_24 )
  {
    TimerIntDisable(  TIMER_BASE_ADDRESS, TIMER_INTERRUPT_SOURCE );
    TimerPrescaleSet( TIMER_BASE_ADDRESS, TIMER_CHANNEL_AB, ( TIMER_PERIOD_TICKS_24 >> 16 ) & 0x0000FFFF);
    TimerLoadSet(     TIMER_BASE_ADDRESS, TIMER_CHANNEL_AB, ( TIMER_PERIOD_TICKS_24       ) & 0x0000FFFF);
    TimerIntClear(    TIMER_BASE_ADDRESS, TIMER_INTERRUPT_SOURCE );
    TimerIntEnable(   TIMER_BASE_ADDRESS, TIMER_INTERRUPT_SOURCE ); 
  }

  void stopTimer( uint32_t TIMER_BASE_ADDRESS, uint32_t TIMER_INTERRUPT_SOURCE )
  {
    TimerIntClear(   TIMER_BASE_ADDRESS, TIMER_INTERRUPT_SOURCE );
    TimerIntDisable( TIMER_BASE_ADDRESS, TIMER_INTERRUPT_SOURCE );
  }

  int pinToTimer( int pin ) ////////////////////////////////////////////////////////
  {       
          if( ( pin == PD_0 ) ||( pin == PA_0 ) ||( pin == PL_4 ) ) { return T0_A; }
    else  if( ( pin == PD_1 ) ||( pin == PA_1 ) ||( pin == PL_5 ) ) { return T0_B; }
    else  if( ( pin == PD_2 ) ||( pin == PA_2 ) ||( pin == PL_6 ) ) { return T1_A; }
    else  if( ( pin == PD_3 ) ||( pin == PA_3 ) ||( pin == PL_7 ) ) { return T1_B; }
    else  if( ( pin == PA_4 ) ||( pin == PM_0 )                   ) { return T2_A; }
    else  if( ( pin == PA_5 ) ||( pin == PM_1 )                   ) { return T2_B; }
    else  if( ( pin == PA_6 ) ||( pin == PM_2 ) ||( pin == PD_4 ) ) { return T3_A; }
    else  if( ( pin == PA_7 ) ||( pin == PM_3 ) ||( pin == PD_5 ) ) { return T3_B; }
    else  if( ( pin == PM_4 ) ||( pin == PB_0 ) ||( pin == PD_6 ) ) { return T4_A; }
    else  if( ( pin == PM_5 ) ||( pin == PB_1 ) ||( pin == PD_7 ) ) { return T4_B; }
    else  if( ( pin == PM_6 ) ||( pin == PB_2 )                   ) { return T5_A; }
    else  if( ( pin == PM_7 ) ||( pin == PB_3 )                   ) { return T5_B; }
    else                                                            { return    0; }
  }

  isrPtr timerIsrPeriodic( int timer ) /////////////////////////////////////////////////////////////////////////////
  {
         if ( timer == T0_A    ) { return timerIsrPeriodic_0A; } // 01 - T0CCP0
    else if ( timer == T0_B    ) { return timerIsrPeriodic_0B; } // 02 - T0CCP1
    else if ( timer == T1_A    ) { return timerIsrPeriodic_1A; } // 03 - T1CCP0
    else if ( timer == T1_B    ) { return timerIsrPeriodic_1B; } // 04 - T1CCP1
    else if ( timer == T2_A    ) { return timerIsrPeriodic_2A; } // 05 - T2CCP0
    else if ( timer == T2_B    ) { return timerIsrPeriodic_2B; } // 06 - T2CCP1
    else if ( timer == T3_A    ) { return timerIsrPeriodic_3A; } // 07 - T3CCP0
    else if ( timer == T3_B    ) { return timerIsrPeriodic_3B; } // 08 - T3CCP1
    else if ( timer == T4_A    ) { return timerIsrPeriodic_4A; } // 09 - T4CCP0
    else if ( timer == T4_B    ) { return timerIsrPeriodic_4B; } // 10 - T4CCP1
    else if ( timer == T5_A    ) { return timerIsrPeriodic_5A; } // 11 - T5CCP0
    else if ( timer == T5_B    ) { return timerIsrPeriodic_5B; } // 12 - T5CCP1
    else if ( timer == T6_A    ) { return timerIsrPeriodic_6A; } // 13 - No CCP pin routed on Launchpad (MCU T6CCP0)
    else if ( timer == T6_B    ) { return timerIsrPeriodic_6B; } // 14 - No CCP pin routed on Launchpad (MCU T6CCP1)
    else if ( timer == T7_A    ) { return timerIsrPeriodic_7A; } // 15 - No CCP pin routed on Launchpad (MCU T7CCP0)
    else if ( timer == T7_B    ) { return timerIsrPeriodic_7B; } // 16 - No CCP pin routed on Launchpad (MCU T7CCP1)
    else                         { return           0; } // INVALID 
  }

  struct TimerHardware timerHardware ( int timer ) /////////////////////////////////////////////////////////////////////////////////////////////////////////
  {
         if ( timer == T0_A    ) { return { SYSCTL_PERIPH_TIMER0, TIMER0_BASE, TIMER_A, INT_TIMER0A }; } // 01 - T0CCP0
    else if ( timer == T0_B    ) { return { SYSCTL_PERIPH_TIMER0, TIMER0_BASE, TIMER_B, INT_TIMER0B }; } // 02 - T0CCP1
    else if ( timer == T1_A    ) { return { SYSCTL_PERIPH_TIMER1, TIMER1_BASE, TIMER_A, INT_TIMER1A }; } // 03 - T1CCP0
    else if ( timer == T1_B    ) { return { SYSCTL_PERIPH_TIMER1, TIMER1_BASE, TIMER_B, INT_TIMER1B }; } // 04 - T1CCP1
    else if ( timer == T2_A    ) { return { SYSCTL_PERIPH_TIMER2, TIMER2_BASE, TIMER_A, INT_TIMER2A }; } // 05 - T2CCP0
    else if ( timer == T2_B    ) { return { SYSCTL_PERIPH_TIMER2, TIMER2_BASE, TIMER_B, INT_TIMER2B }; } // 06 - T2CCP1
    else if ( timer == T3_A    ) { return { SYSCTL_PERIPH_TIMER3, TIMER3_BASE, TIMER_A, INT_TIMER3A }; } // 07 - T3CCP0
    else if ( timer == T3_B    ) { return { SYSCTL_PERIPH_TIMER3, TIMER3_BASE, TIMER_B, INT_TIMER3B }; } // 08 - T3CCP1
    else if ( timer == T4_A    ) { return { SYSCTL_PERIPH_TIMER4, TIMER4_BASE, TIMER_A, INT_TIMER4A }; } // 09 - T4CCP0
    else if ( timer == T4_B    ) { return { SYSCTL_PERIPH_TIMER4, TIMER4_BASE, TIMER_B, INT_TIMER4B }; } // 10 - T4CCP1
    else if ( timer == T5_A    ) { return { SYSCTL_PERIPH_TIMER5, TIMER5_BASE, TIMER_A, INT_TIMER5A }; } // 11 - T5CCP0
    else if ( timer == T5_B    ) { return { SYSCTL_PERIPH_TIMER5, TIMER5_BASE, TIMER_B, INT_TIMER5B }; } // 12 - T5CCP1
    else if ( timer == T6_A    ) { return { SYSCTL_PERIPH_TIMER6, TIMER6_BASE, TIMER_A, INT_TIMER6A }; } // 13 - No CCP pin routed on Launchpad (MCU T6CCP0)
    else if ( timer == T6_B    ) { return { SYSCTL_PERIPH_TIMER6, TIMER6_BASE, TIMER_B, INT_TIMER6B }; } // 14 - No CCP pin routed on Launchpad (MCU T6CCP1)
    else if ( timer == T7_A    ) { return { SYSCTL_PERIPH_TIMER7, TIMER7_BASE, TIMER_A, INT_TIMER7A }; } // 15 - No CCP pin routed on Launchpad (MCU T7CCP0)
    else if ( timer == T7_B    ) { return { SYSCTL_PERIPH_TIMER7, TIMER7_BASE, TIMER_B, INT_TIMER7B }; } // 16 - No CCP pin routed on Launchpad (MCU T7CCP1)
    else                         { return {                    0,           0,       0,           0 }; } // INVALID 
  }

   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Todo...Note: This function was never implemented by the TivaWare DriverLib team for some reason, TimerValueGet returns a 16 bit timer, not the full 24 bit timer
  uint32_t TimerValueGet24( uint32_t timer_base, uint32_t timer_channel_ab ) 
  { 
    if (       timer_channel_ab == TIMER_A ) { return (uint32_t)( HWREG( timer_base + TIMER_O_TAR ) + ( HWREG( timer_base + TIMER_O_TAPS ) << 16 ) ); } 
    else if (  timer_channel_ab == TIMER_B ) { return (uint32_t)( HWREG( timer_base + TIMER_O_TBR ) + ( HWREG( timer_base + TIMER_O_TBPS ) << 16 ) ); } 
  }

}// end namespace roveware ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
