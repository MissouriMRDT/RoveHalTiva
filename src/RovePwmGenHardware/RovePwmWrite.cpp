///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MRDT 2019 => TivaC1294/TivaC129E Pin Module Pulse Width Modulation Generator (PWM Gen)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "rovePwmWrite.h"

#include "rovePwmGenHardware.h"

#include "Energia.h"

#include "driverlib/pwm.h"

#include "stdint.h"

uint32_t todo_clock_div = PWM_SYSCLK_DIV_64;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void rovePwmWrite( int pin, int pulse_width_micros, int pulse_period_micros) //////////////////////////////////////////////////////////////////////////////////////////////////////
{
  uint32_t  pulse_width_ticks_16   = ( pulse_width_micros  * roveware::PWM_GEN_DIV_1_TICKS_PER_MICRO ) / 64; // Todo clock_div? enum hmmm...
  uint32_t  pulse_period_ticks_16  = ( pulse_period_micros * roveware::PWM_GEN_DIV_1_TICKS_PER_MICRO ) / 64;

       if ( pulse_width_ticks_16 == 0  ) 
       {    digitalWrite( pin, LOW  ); }

  else if ( pulse_width_ticks_16 >= pulse_period_ticks_16 ) 
  {         digitalWrite( pin, HIGH ); }
  else 
  {         struct roveware::PwmGenHardware Hardware = roveware::pwmGenHardware( pin );

            roveware::setPwmGen   ( Hardware.PWM_GEN_PIN_MUX, Hardware.PWM_GEN, todo_clock_div,              Hardware.PORT_BASE_ADDRESS, Hardware.PIN_BIT_MASK );
            roveware::pwmGenWrite ( Hardware.PWM_GEN,         Hardware.PWM_OUT, Hardware.PIN_BIT_MASK, pulse_width_ticks_16,             pulse_period_ticks_16 );
  }
}

//////////////////////////////////////////////////////////////////////////

void rovePwmGenAnalogWrite( int pin,        int value) /////////////////
{    rovePwmWrite(              pin,  ( (2040 * value) / 255 ), 2040 ); }