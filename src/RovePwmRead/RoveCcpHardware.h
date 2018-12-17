///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MRDT Summer 2018 => Tiva C Pin Module Pulse Width Modulation Generator (PWM Gen)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ROVE_CCP_HARDWARE_H
#define ROVE_CCP_HARDWARE_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TxCCPx                                => Tiva C Series TM4C1294NCPDT Microcontroller Data Sheet (Rev. B) defines the module top down register hierarchy
//                                       => ti.com/lit/ds/symlink/tm4c1294ncpdt.pdf

// GPIO_PORTx_BASE, GPIO_PIN_x, PX_x     => Energia provides Wiring api lookup of TivaWare gpio port values by Energia pin_number number
//                                       => github.com/energia/Energia/blob/master/hardware/lm4f/variants/launchpad_129/pins_energia.h

// SYSCTL_PERIPH_TIMERX,GPIO_Pxx_TxCCPx.. => TivaWare Peripheral Driver Library provides pin_number peripheral mux, timer base, capture periph, and interrupt masks
// TIMERX_BASE, INT_TIMERXA, INT_TIMERXB => ti.com/lit/ug/spmu298d/spmu298d.pdf   

// TivaC_1294 support => Wiring 'style' Api,  progmem lookup table, conforming to the Arduino / Energia pin_map pattern

//////////////////////////////

#include "RoveTimerHardware.h"
#include "RoveRingBuff.h"

#include "stdint.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PIOSC                  => 13.3.2 Timer Clock Source
// CCP                    => 13.4 Initialization and Configuration
// Edge Time              => 13.4.4 Input Edge Time Mode
// SYSCLK_DIV             => 13-5. 16-Bit Timer With Prescaler Configurations

// Clock Divide by 1 ~ 64 on a 16b generator with 8bit prescale (16+8=24) at 120Mhz system clock => ? nS ~ ? mS period => 120Mhz ~ ?Hz frequency
//      => 1 micros < ?      < ? micros
//   && => 0 micros < ?      < ? micros
// else => we default to 
// defualts to TODO on rovePwmRead and rovePwmReadTicks24

namespace roveware /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
  struct CcpHardware ////////////////////
  {
    volatile uint32_t CCP_PIN_MUX;
    volatile uint32_t PORT_BASE_ADDRESS;
    volatile uint32_t PIN_BIT_MASK;
  };

  //////////////////////////////////////////////////////////////////////////////////////////////////
  //template<class BUFF_TYPE_T, size_t BUFF_SIZE_T> // todo 
  
  struct CcpTicks
  {  
    struct CcpHardware Hardware;
    struct Timer       Timer;
    int    timer;

    volatile int      priority;
    volatile uint32_t last_capture_ticks;
    volatile bool     last_digital_read;

    RoveRingBuff < uint32_t, 16 > PulsePeriodsamples; // RoveRingBuff < TYPE, SIZE > PulsePeriodsamples; // todo make this whole struct a template struct on BUFF_TYPE_T, BUFF_SIZE_T
    RoveRingBuff < uint32_t, 16 > PulseWidthSamples;  // RoveRingBuff < TYPE, SIZE > PulseWidthSamples;   
  };

  struct CcpWireBreak
  {
    volatile bool edge_captured_T0_A:1;
    volatile bool edge_captured_T0_B:1;
    volatile bool edge_captured_T1_A:1;
    volatile bool edge_captured_T1_B:1;
    volatile bool edge_captured_T2_A:1;
    volatile bool edge_captured_T2_B:1;
    volatile bool edge_captured_T3_A:1;
    volatile bool edge_captured_T3_B:1;
    volatile bool edge_captured_T4_A:1;
    volatile bool edge_captured_T4_B:1;
    volatile bool edge_captured_T5_A:1;
    volatile bool edge_captured_T5_B:1; 
    
    volatile bool wire_broken_T0_A:1;
    volatile bool wire_broken_T0_B:1;
    volatile bool wire_broken_T1_A:1;
    volatile bool wire_broken_T1_B:1;
    volatile bool wire_broken_T2_A:1;
    volatile bool wire_broken_T2_B:1;
    volatile bool wire_broken_T3_A:1;
    volatile bool wire_broken_T3_B:1;
    volatile bool wire_broken_T4_A:1;
    volatile bool wire_broken_T4_B:1;
    volatile bool wire_broken_T5_A:1;
    volatile bool wire_broken_T5_B:1; 
  };

  void ccpIsrTicks_0A ( void );
  void ccpIsrTicks_0B ( void );
  void ccpIsrTicks_1A ( void );
  void ccpIsrTicks_1B ( void );
  void ccpIsrTicks_2A ( void );
  void ccpIsrTicks_2B ( void );
  void ccpIsrTicks_3A ( void );
  void ccpIsrTicks_3B ( void );
  void ccpIsrTicks_4A ( void );
  void ccpIsrTicks_4B ( void );
  void ccpIsrTicks_5A ( void );
  void ccpIsrTicks_5B ( void );
  void ccpIsrWireBreak( void );

  bool           isCcpValid(          int pin );
  void           attachCcpTicks(      int timer, 
                                      struct CcpTicks* Ccp );
  void           dispatchCcpIsrTicks( struct CcpTicks* Ccp );
  uint32_t       ccpPinMux(           int pin );
  isrPtr         ccpIsrTicks(         int timer );

  void ccpEdgeIsCaptured( int timer, bool edge_captured );
  bool isCcpEdgeCaptured( int timer );
  void ccpWireIsBroken(   int timer, bool wirebreak );
  bool isCcpWireBroken(   int timer );

}// end namespace roveware /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // ROVE_CCP_HARDWARE_H

