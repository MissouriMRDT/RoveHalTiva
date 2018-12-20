///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MRDT 2019 => TivaC1294/TivaC129E Pin Module Pulse Width Modulation Generator (PWM Gen)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "RovePwmRead.h"
#include "RoveTimerNumbers.h"
#include "RoveCcpHardware.h"
#include "RoveRingBuff.h"

#include "Energia.h"
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

#include "stdint.h"

RovePwmRead::RovePwmRead( int pin, int priority ) /////////////////////////////////////////////////////////////////////
{
  this->pin          = pin;
  this->Ccp.priority = priority;
  this->Ccp.timer    = roveware::pinToTimer( this->pin );

  if ( ( this->Ccp.timer %  2) == 0 ){ this->Ccp.Timer.interrupt_source = TIMER_CFG_B_CAP_TIME; }
  else {                               this->Ccp.Timer.interrupt_source = TIMER_CFG_A_CAP_TIME; }

  this->Ccp.Hardware.PORT_BASE_ADDRESS = (uint32_t)portBASERegister( digitalPinToPort( this->pin ) ); // Energia.h
  this->Ccp.Hardware.PIN_BIT_MASK      = digitalPinToBitMask(                          this->pin   ); // Energia.h
  this->Ccp.Hardware.CCP_PIN_MUX       = roveware::ccpPinMux(                          this->pin   );
  this->Ccp.Timer.Hardware             = roveware::timerHardware(                      this->Ccp.timer );
  this->Ccp.Timer.timerIsr             = roveware::ccpIsrTicks(                        this->Ccp.timer );
  roveware::attachCcpTicks( this->Ccp.timer, &(this->Ccp) );
}

void RovePwmRead::start() ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{ if ( roveware::isTimerValid( this->Ccp.timer ) )
  {    
    this->Ccp.Timer.period_ticks = 16777215; // 2^24 - 1
    pinMode(                  this->pin, INPUT); // todo pull this part out into drivelib calls?
    GPIOPinTypeTimer(         this->Ccp.Hardware.PORT_BASE_ADDRESS, 
                              this->Ccp.Hardware.PIN_BIT_MASK); // move to a Gpio struct?
    GPIOPinConfigure(         this->Ccp.Hardware.CCP_PIN_MUX );
    
    uint32_t TIMER_CHANNEL_AB;
    if ( ( this->Ccp.timer %  2) == 0 ){ TIMER_CHANNEL_AB = TIMER_B; }
    else                               { TIMER_CHANNEL_AB = TIMER_A; }
                   
    roveware::configureTimer( roveware::TIMER_USE_PIOSC,
                              roveware::TIMER_USE_CAPTURE_TICKS_AB,
                              this->Ccp.Timer.Hardware.TIMER_PERIPHERAL,
                              this->Ccp.Timer.Hardware.TIMER_BASE_ADDRESS, 
                              TIMER_CHANNEL_AB );
    TimerControlEvent(        this->Ccp.Timer.Hardware.TIMER_BASE_ADDRESS,
                              this->Ccp.Timer.Hardware.TIMER_CHANNEL_AB,
                              TIMER_EVENT_BOTH_EDGES);
    roveware::attachTimerIsr( this->Ccp.Timer.Hardware.TIMER_BASE_ADDRESS,
                              this->Ccp.Timer.Hardware.TIMER_CHANNEL_AB,
                              this->Ccp.Timer.interrupt_source,
                              this->Ccp.Timer.Hardware.TIMER_INTERRUPT,
                              this->Ccp.Timer.timerIsr, 
                              this->Ccp.priority );
    roveware::startTimer(     this->Ccp.Timer.Hardware.TIMER_BASE_ADDRESS,
                              this->Ccp.Timer.Hardware.TIMER_CHANNEL_AB,
                              this->Ccp.Timer.interrupt_source,
                              this->Ccp.Timer.period_ticks ); }
}

void RovePwmRead::stop() ////////////////////////////////////////////////////////////////////////////////////////
{ if ( roveware::isTimerValid( this->Ccp.timer ) ) 
  {    roveware::stopTimer(    this->Ccp.Timer.Hardware.TIMER_BASE_ADDRESS, this->Ccp.Timer.interrupt_source ); }
}

bool  RovePwmRead::isWireBroken( ) ////////////////////////////////////////
{ return roveware::isCcpWireBroken( roveware::pinToTimer( this->pin ) ); }

int RovePwmRead::readWidth( ) ///
{ return Ccp.PulseWidths.avg(); }

int RovePwmRead::readPeriod( ) ///
{ return Ccp.PulsePeriods.avg(); }

////////////////////////////////

void rovePwmReadIsrWireBreak() //
{  roveware::ccpIsrWireBreak(); }

