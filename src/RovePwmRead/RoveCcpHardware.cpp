///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MRDT Summer 2018 => Tiva C Pin Module Pulse Width Modulation Generator (PWM Gen)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TxCCPx                                => Tiva C Series TM4C1294NCPDT Microcontroller Data Sheet (Rev. B) defines the module top down register hierarchy
//                                       => ti.com/lit/ds/symlink/tm4c1294ncpdt.pdf

// GPIO_PORTx_BASE, GPIO_PIN_x, PX_x     => Energia provides Wiring api lookup of TivaWare gpio port values by Energia pin_number number
//                                       => github.com/energia/Energia/blob/master/hardware/lm4f/variants/launchpad_129/pins_energia.h

// SYSCTL_PERIPH_TIMERX,GPIO_Pxx_TxCCPx.. => TivaWare Peripheral Driver Library provides pin_number peripheral mux, timer base, capture periph, and interrupt masks
// TIMERX_BASE, INT_TIMERXA, INT_TIMERXB => ti.com/lit/ug/spmu298d/spmu298d.pdf   

// TivaC_1294 support => Wiring 'style' Api,  progmem lookup table, conforming to the Arduino / Energia pin_map pattern

//////////////////////////////

#include "RoveCcpHardware.h"
#include "RoveTimerNumbers.h"
#include "RoveTimerHardware.h"

#include "stdint.h"

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

#include <stdint.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Table 13-2. General-Purpose Timers Signals (128TQFP)

namespace roveware ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
  struct CcpTicks* CcpTicks[12] = { };

  struct CcpWireBreak CcpWireBreak = { false, false, false, false, false, false, 
                                       false, false, false, false, false, false,  // edge_captured = false
                                       true,  true,  true,  true,  true,  true,  
                                       true,  true,  true,  true,  true,  true }; // wire_broken   = true

  volatile bool ccp_wire_break_is_init = false;

  void ccpIsrTicks_0A ( void ) { dispatchCcpIsrTicks( CcpTicks[ T0_A - 1] ); }
  void ccpIsrTicks_0B ( void ) { dispatchCcpIsrTicks( CcpTicks[ T0_B - 1] ); }
  void ccpIsrTicks_1A ( void ) { dispatchCcpIsrTicks( CcpTicks[ T1_A - 1] ); }
  void ccpIsrTicks_1B ( void ) { dispatchCcpIsrTicks( CcpTicks[ T1_B - 1] ); }
  void ccpIsrTicks_2A ( void ) { dispatchCcpIsrTicks( CcpTicks[ T2_A - 1] ); }
  void ccpIsrTicks_2B ( void ) { dispatchCcpIsrTicks( CcpTicks[ T2_B - 1] ); }
  void ccpIsrTicks_3A ( void ) { dispatchCcpIsrTicks( CcpTicks[ T3_A - 1] ); }
  void ccpIsrTicks_3B ( void ) { dispatchCcpIsrTicks( CcpTicks[ T3_B - 1] ); }
  void ccpIsrTicks_4A ( void ) { dispatchCcpIsrTicks( CcpTicks[ T4_A - 1] ); }
  void ccpIsrTicks_4B ( void ) { dispatchCcpIsrTicks( CcpTicks[ T4_B - 1] ); }
  void ccpIsrTicks_5A ( void ) { dispatchCcpIsrTicks( CcpTicks[ T5_A - 1] ); }
  void ccpIsrTicks_5B ( void ) { dispatchCcpIsrTicks( CcpTicks[ T5_B - 1] ); }

  bool isCcpValid( int pin ) //////////
  { return ccpPinMux(  pin ) && true; }

  void attachCcpTicks( int timer, struct CcpTicks* Ccp )
  {             CcpTicks [ timer - 1 ] = Ccp; }


  volatile bool edge_capture_blink = LOW; 

  void dispatchCcpIsrTicks( struct CcpTicks* Ccp ) ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  {
    TimerIntClear(    Ccp->Timer.Hardware.TIMER_BASE_ADDRESS, Ccp->Timer.interrupt_source );
    TimerIntDisable(  Ccp->Timer.Hardware.TIMER_BASE_ADDRESS, Ccp->Timer.interrupt_source  ); // todo switch the prescaler to first everywhere?
    TimerPrescaleSet( Ccp->Timer.Hardware.TIMER_BASE_ADDRESS, Ccp->Timer.Hardware.TIMER_CHANNEL_AB, (uint8_t)(Ccp->Timer.period_ticks >> 16));      //prescale takes the last 8 bits
    TimerLoadSet(     Ccp->Timer.Hardware.TIMER_BASE_ADDRESS, Ccp->Timer.Hardware.TIMER_CHANNEL_AB,          (Ccp->Timer.period_ticks  & 0xFFFF));  //load register only holds first 16 bits
    
    uint32_t capture_ticks = TimerValueGet24( Ccp->Timer.Hardware.TIMER_BASE_ADDRESS, Ccp->Timer.Hardware.TIMER_CHANNEL_AB );
    bool digital_read  = GPIOPinRead( Ccp->Hardware.PORT_BASE_ADDRESS, Ccp->Hardware.PIN_BIT_MASK ) && true;
  //HWREG( Ccp->Hardware.PORT_BASE_ADDRESS + (GPIO_O_DATA + ( Ccp->Hardware.PIN_BIT_MASK << 2 )) ); // todo wut 2?

    if ( ( digital_read == HIGH ) and ( digital_read != Ccp->last_digital_read ) )
    {
      //Ccp->PulseWidths.push(   capture_ticks );
      //Ccp->PulsePeriods.push(  capture_ticks + Ccp->last_capture_ticks ); // Todo wire break clear on valid handling
      ccpEdgeIsCaptured( Ccp->timer, true ); // success
    }

    Ccp->last_capture_ticks = capture_ticks;
    Ccp->last_digital_read  = digital_read;

    digitalWrite( PN_1,  digital_read );

    TimerIntClear(  Ccp->Timer.Hardware.TIMER_BASE_ADDRESS, Ccp->Timer.interrupt_source );
    TimerIntEnable( Ccp->Timer.Hardware.TIMER_BASE_ADDRESS, Ccp->Timer.interrupt_source );
  }

  void ccpIsrWireBreak( void ) /////////////////////////////
  {
    ccpWireIsBroken( T0_A, !isCcpEdgeCaptured( T0_A ) );
    ccpWireIsBroken( T0_B, !isCcpEdgeCaptured( T0_B ) );
    ccpWireIsBroken( T1_A, !isCcpEdgeCaptured( T1_A ) );
    ccpWireIsBroken( T1_B, !isCcpEdgeCaptured( T1_B ) );
    ccpWireIsBroken( T2_A, !isCcpEdgeCaptured( T2_A ) );
    ccpWireIsBroken( T2_B, !isCcpEdgeCaptured( T2_B ) );
    ccpWireIsBroken( T3_A, !isCcpEdgeCaptured( T3_A ) );
    ccpWireIsBroken( T3_B, !isCcpEdgeCaptured( T3_B ) );
    ccpWireIsBroken( T4_A, !isCcpEdgeCaptured( T4_A ) );
    ccpWireIsBroken( T4_B, !isCcpEdgeCaptured( T4_B ) );
    ccpWireIsBroken( T5_A, !isCcpEdgeCaptured( T5_A ) );
    ccpWireIsBroken( T5_B, !isCcpEdgeCaptured( T5_B ) );
    
    ccpEdgeIsCaptured( T0_A, false ); 
    ccpEdgeIsCaptured( T0_B, false ); 
    ccpEdgeIsCaptured( T1_A, false ); 
    ccpEdgeIsCaptured( T1_B, false ); 
    ccpEdgeIsCaptured( T2_A, false ); 
    ccpEdgeIsCaptured( T2_B, false ); 
    ccpEdgeIsCaptured( T3_A, false ); 
    ccpEdgeIsCaptured( T3_B, false ); 
    ccpEdgeIsCaptured( T4_A, false ); 
    ccpEdgeIsCaptured( T4_B, false ); 
    ccpEdgeIsCaptured( T5_A, false ); 
    ccpEdgeIsCaptured( T5_B, false );  
  }


  void ccpEdgeIsCaptured( int timer, bool edge_captured ) ///////////////////////
  {      if ( timer == T0_A ){ CcpWireBreak.edge_captured_T0_A = edge_captured; }
    else if ( timer == T0_B ){ CcpWireBreak.edge_captured_T0_B = edge_captured; }
    else if ( timer == T1_A ){ CcpWireBreak.edge_captured_T1_A = edge_captured; }
    else if ( timer == T1_B ){ CcpWireBreak.edge_captured_T1_B = edge_captured; }
    else if ( timer == T2_A ){ CcpWireBreak.edge_captured_T2_A = edge_captured; }
    else if ( timer == T2_B ){ CcpWireBreak.edge_captured_T2_B = edge_captured; }
    else if ( timer == T3_A ){ CcpWireBreak.edge_captured_T3_A = edge_captured; }
    else if ( timer == T3_B ){ CcpWireBreak.edge_captured_T3_B = edge_captured; }
    else if ( timer == T4_A ){ CcpWireBreak.edge_captured_T4_A = edge_captured; }
    else if ( timer == T4_B ){ CcpWireBreak.edge_captured_T4_B = edge_captured; }
    else if ( timer == T5_A ){ CcpWireBreak.edge_captured_T5_A = edge_captured; }
    else if ( timer == T5_B ){ CcpWireBreak.edge_captured_T5_B = edge_captured; }
  }

  bool isCcpEdgeCaptured( int timer ) ///////////////////////////////////
  {      if ( timer == T0_A ) { return CcpWireBreak.edge_captured_T0_A; }
    else if ( timer == T0_B ) { return CcpWireBreak.edge_captured_T0_B; }
    else if ( timer == T1_A ) { return CcpWireBreak.edge_captured_T1_A; }
    else if ( timer == T1_B ) { return CcpWireBreak.edge_captured_T1_B; }
    else if ( timer == T2_A ) { return CcpWireBreak.edge_captured_T2_A; }
    else if ( timer == T2_B ) { return CcpWireBreak.edge_captured_T2_B; }
    else if ( timer == T3_A ) { return CcpWireBreak.edge_captured_T3_A; }
    else if ( timer == T3_B ) { return CcpWireBreak.edge_captured_T3_B; }
    else if ( timer == T4_A ) { return CcpWireBreak.edge_captured_T4_A; }
    else if ( timer == T4_B ) { return CcpWireBreak.edge_captured_T4_B; }
    else if ( timer == T5_A ) { return CcpWireBreak.edge_captured_T5_A; }
    else if ( timer == T5_B ) { return CcpWireBreak.edge_captured_T5_B; }
    else                      { return                           false; }
  }

  void ccpWireIsBroken( int timer, bool wirebreak ) ///////////////////////
  {      if ( timer == T0_A ){ CcpWireBreak.wire_broken_T0_A = wirebreak; }
    else if ( timer == T0_B ){ CcpWireBreak.wire_broken_T0_B = wirebreak; }
    else if ( timer == T1_A ){ CcpWireBreak.wire_broken_T1_A = wirebreak; }
    else if ( timer == T1_B ){ CcpWireBreak.wire_broken_T1_B = wirebreak; }
    else if ( timer == T2_A ){ CcpWireBreak.wire_broken_T2_A = wirebreak; }
    else if ( timer == T2_B ){ CcpWireBreak.wire_broken_T2_B = wirebreak; }
    else if ( timer == T3_A ){ CcpWireBreak.wire_broken_T3_A = wirebreak; }
    else if ( timer == T3_B ){ CcpWireBreak.wire_broken_T3_B = wirebreak; }
    else if ( timer == T4_A ){ CcpWireBreak.wire_broken_T4_A = wirebreak; }
    else if ( timer == T4_B ){ CcpWireBreak.wire_broken_T4_B = wirebreak; }
    else if ( timer == T5_A ){ CcpWireBreak.wire_broken_T5_A = wirebreak; }
    else if ( timer == T5_B ){ CcpWireBreak.wire_broken_T5_B = wirebreak; }
  }

  bool isCcpWireBroken( int timer ) ///////////////////////////////////
  {      if ( timer == T0_A ) { return CcpWireBreak.wire_broken_T0_A; }
    else if ( timer == T0_B ) { return CcpWireBreak.wire_broken_T0_B; }
    else if ( timer == T1_A ) { return CcpWireBreak.wire_broken_T1_A; }
    else if ( timer == T1_B ) { return CcpWireBreak.wire_broken_T1_B; }
    else if ( timer == T2_A ) { return CcpWireBreak.wire_broken_T2_A; }
    else if ( timer == T2_B ) { return CcpWireBreak.wire_broken_T2_B; }
    else if ( timer == T3_A ) { return CcpWireBreak.wire_broken_T3_A; }
    else if ( timer == T3_B ) { return CcpWireBreak.wire_broken_T3_B; }
    else if ( timer == T4_A ) { return CcpWireBreak.wire_broken_T4_A; }
    else if ( timer == T4_B ) { return CcpWireBreak.wire_broken_T4_B; }
    else if ( timer == T5_A ) { return CcpWireBreak.wire_broken_T5_A; }
    else if ( timer == T5_B ) { return CcpWireBreak.wire_broken_T5_B; }
    else                      { return                          true; } 
  }

  uint32_t ccpPinMux( int pin ) ////////////////////////////////
  {
         if( pin == PD_3 ) { return GPIO_PD3_T1CCP1; } // pin 07
    else if( pin == PB_2 ) { return GPIO_PB2_T5CCP0; } // pin 09
    else if( pin == PB_3 ) { return GPIO_PB3_T5CCP1; } // pin 10
    else if( pin == PD_0 ) { return GPIO_PD0_T0CCP0; } // pin 14
    else if( pin == PD_1 ) { return GPIO_PD1_T0CCP1; } // pin 15
    else if( pin == PM_3 ) { return GPIO_PM3_T3CCP1; } // pin 19
    else if( pin == PD_7 ) { return GPIO_PD7_T4CCP1; } // pin 27
    else if( pin == PA_6 ) { return GPIO_PA6_T3CCP0; } // pin 28
    else if( pin == PM_4 ) { return GPIO_PM4_T4CCP0; } // pin 29
    else if( pin == PM_5 ) { return GPIO_PM5_T4CCP1; } // pin 30
    else if( pin == PL_5 ) { return GPIO_PL5_T0CCP1; } // pin 35
    else if( pin == PL_4 ) { return GPIO_PL4_T0CCP0; } // pin 36
    else if( pin == PD_2 ) { return GPIO_PD2_T1CCP0; } // pin 42
    else if( pin == PD_4 ) { return GPIO_PD4_T3CCP0; } // pin 45
    else if( pin == PD_5 ) { return GPIO_PD5_T3CCP1; } // pin 46
    else if( pin == PM_6 ) { return GPIO_PM6_T5CCP0; } // pin 51
    else if( pin == PA_7 ) { return GPIO_PA7_T3CCP1; } // pin 57
    else if( pin == PM_7 ) { return GPIO_PM7_T5CCP1; } // pin 59
    else if( pin == PA_4 ) { return GPIO_PA4_T2CCP0; } // pin 69
    else if( pin == PA_5 ) { return GPIO_PA5_T2CCP1; } // pin 70
    else if( pin == PM_2 ) { return GPIO_PA5_T2CCP1; } // pin 75
    else if( pin == PM_1 ) { return GPIO_PM1_T2CCP1; } // pin 76
    else if( pin == PM_0 ) { return GPIO_PM0_T2CCP0; } // pin 77
    else if( pin == PD_6 ) { return GPIO_PD6_T4CCP0; } // pin 87
    else if( pin == PA_0 ) { return GPIO_PA0_T0CCP0; } // pin 88
    else if( pin == PA_1 ) { return GPIO_PA1_T0CCP1; } // pin 89
    else if( pin == PA_2 ) { return GPIO_PA2_T1CCP0; } // pin 90
    else if( pin == PA_3 ) { return GPIO_PA3_T1CCP1; } // pin 91
    else if( pin == PL_6 ) { return GPIO_PL6_T1CCP0; } // pin 92
    else if( pin == PL_7 ) { return GPIO_PL7_T1CCP1; } // pin 93
    else if( pin == PB_0 ) { return GPIO_PB0_T4CCP0; } // pin 94
    else if( pin == PB_1 ) { return GPIO_PB1_T4CCP1; } // pin 95
    else                   { return               0; } // INVALID
  }

  isrPtr ccpIsrTicks( int timer ) ////////////////////////////////////
  {
         if ( timer == T0_A ){ return ccpIsrTicks_0A; } // 01 - T0CCP0
    else if ( timer == T0_B ){ return ccpIsrTicks_0B; } // 02 - T0CCP1
    else if ( timer == T1_A ){ return ccpIsrTicks_1A; } // 03 - T1CCP0
    else if ( timer == T1_B ){ return ccpIsrTicks_1B; } // 04 - T1CCP1
    else if ( timer == T2_A ){ return ccpIsrTicks_2A; } // 05 - T2CCP0
    else if ( timer == T2_B ){ return ccpIsrTicks_2B; } // 06 - T2CCP1
    else if ( timer == T3_A ){ return ccpIsrTicks_3A; } // 07 - T3CCP0
    else if ( timer == T3_B ){ return ccpIsrTicks_3B; } // 08 - T3CCP1
    else if ( timer == T4_A ){ return ccpIsrTicks_4A; } // 09 - T4CCP0
    else if ( timer == T4_B ){ return ccpIsrTicks_4B; } // 10 - T4CCP1
    else if ( timer == T5_A ){ return ccpIsrTicks_5A; } // 11 - T5CCP0
    else if ( timer == T5_B ){ return ccpIsrTicks_5B; } // 12 - T5CCP1
    else                     { return              0; } // INVALID 
  }

}// end namespace roveware ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////