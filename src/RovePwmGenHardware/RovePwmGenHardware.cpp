///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MRDT 2019 => TivaC1294/TivaC129E Pin Module Pulse Width Modulation Generator (PWM Gen)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "RovePwmGenHardware.h"

#include "Energia.h"

#include "inc/hw_memmap.h"

#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

#include "tm4c1294ncpdt.h"

#include "stdint.h"

const uint8_t INVALID = 0; // todo

namespace roveware /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
  bool pwm_gen_periph_begun;

  struct PwmGenHardware pwmGenHardware ( int pin )
  {
         if ( pin == PF_0    ) { return { GPIO_PF0_M0PWM0, PWM_GEN_0, PWM_OUT_0, GPIO_PORTF_BASE, (1 << 0) }; } 
    else if ( pin == PF_1    ) { return { GPIO_PF1_M0PWM1, PWM_OUT_1, PWM_OUT_1, GPIO_PORTF_BASE, (1 << 1) }; } 
    else if ( pin == PF_2    ) { return { GPIO_PF2_M0PWM2, PWM_GEN_1, PWM_OUT_2, GPIO_PORTF_BASE, (1 << 2) }; } 
    else if ( pin == PF_3    ) { return { GPIO_PF3_M0PWM3, PWM_GEN_1, PWM_OUT_3, GPIO_PORTF_BASE, (1 << 3) }; } 
    else if ( pin == PG_0    ) { return { GPIO_PG0_M0PWM4, PWM_GEN_2, PWM_OUT_4, GPIO_PORTG_BASE, (1 << 0) }; } 
    else if ( pin == PG_1    ) { return { GPIO_PG1_M0PWM5, PWM_GEN_2, PWM_OUT_5, GPIO_PORTG_BASE, (1 << 1) }; } 
    else if ( pin == PK_4    ) { return { GPIO_PK4_M0PWM6, PWM_GEN_3, PWM_OUT_6, GPIO_PORTK_BASE, (1 << 4) }; } 
    else if ( pin == PK_5    ) { return { GPIO_PK5_M0PWM7, PWM_GEN_3, PWM_OUT_7, GPIO_PORTK_BASE, (1 << 5) }; } 
    else                       { return {         INVALID,   INVALID,   INVALID,         INVALID,  INVALID }; } 
  }

  void setPwmGen ( uint32_t PWM_PIN_MUX, uint32_t PWM_GEN, uint32_t CLOCK_DIV, uint32_t PORT_BASE_ADDRESS, uint32_t PIN_BIT_MASK )
  {
    if (      pwm_gen_periph_begun == false )
    {         pwm_gen_periph_begun  = true;
              SysCtlPeripheralEnable( SYSCTL_PERIPH_PWM0 );
      while (!SysCtlPeripheralReady(  SYSCTL_PERIPH_PWM0 ))
      {   };
    }

    PWMClockSet(        PWM0_BASE, CLOCK_DIV );
    PWMGenConfigure(    PWM0_BASE, PWM_GEN, PWM_GEN_MODE_NO_SYNC | PWM_GEN_MODE_DOWN );
    PWMDeadBandDisable( PWM0_BASE, PWM_GEN );  // todo remove? this is already disabled by default?
    PWMGenEnable(       PWM0_BASE, PWM_GEN );
 
    GPIOPinConfigure(   PWM_PIN_MUX );
    GPIOPinTypePWM(     PORT_BASE_ADDRESS, PIN_BIT_MASK );
  }

  void pwmGenWrite ( uint32_t PWM_GEN, uint32_t PWM_OUT, uint32_t PIN_BIT_MASK, uint16_t WIDTH_TICKS_16, uint16_t PERIOD_TICKS_16 )
  {
    PWMGenPeriodSet(  PWM0_BASE, PWM_GEN,      PERIOD_TICKS_16 );
    PWMPulseWidthSet( PWM0_BASE, PWM_OUT,      WIDTH_TICKS_16  );
    PWMOutputState(   PWM0_BASE, PIN_BIT_MASK, true ); 
  }

}// end namespace roveware ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////