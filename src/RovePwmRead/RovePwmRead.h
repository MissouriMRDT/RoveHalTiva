///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MRDT 2019 => TivaC1294/TivaC129E Pin Module Pulse Width Modulation Generator (PWM Gen)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ROVE_PWM_READ_H
#define ROVE_PWM_READ_H

#include "stdint.h"
#include "RoveCcpHardware.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Timer                  => ti.com/lit/ds/symlink/tm4c1294ncpdt.pdf => 13-2.    General-Purpose Timers Signals
// Edge Time Capture Pins => ti.com/lit/ds/symlink/tm4c1294ncpdt.pdf => 13.3.3.4 Input Edge-Time Mode

// PD_0, PA_0, PL_4     =>     T0_A     PIN_CONFLICTS Energia::AnalogWrite or roveware::pwmRead
// PD_1, PA_1, PL_5     =>     T0_B     PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
// PD_2, PA_2, PL_6     =>     T1_A     PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead 
// PD_3, PA_3, PL_7     =>     T1_B     PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
// PA_4, PM_0           =>     T2_A     PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead or Servo
// PA_5, PM_1           =>     T2_B     PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
// PA_6, PM_2, PD_4     =>     T3_A     PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
// PA_7, PM_3, PD_5     =>     T3_B     PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
// PM_4, PB_0, PD_6     =>     T4_A     PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
// PM_5, PB_1, PD_7     =>     T4_B     PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
// PM_6, PB_2           =>     T5_A     PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
// PM_7, PB_3           =>     T5_B     PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead

class RovePwmRead ///////////////////////////////////
{
public:

  RovePwmRead( int pin, int priority=7 );
  int read(); // Duty in millipercent

//int readPulseWidthMicros();
//int readPulsePeriodMicros();
  void start();
  void stop();

private:
  uint8_t  pin;
  struct   roveware::CcpTicks Ccp; // todo => wrap the ring buffer size template one level higher => roveware::CcpTicks < uint32_t, 16 > Ccp;
};

void wireBreakTimeout( );
bool isWireBroken( int pin );

// todo => all this goes IN roveware:: and then one more level of RoveHal include wrapper?
// todo Template the buffer size?

#endif // ROVE_PWM_READ_H