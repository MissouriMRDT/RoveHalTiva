/////////////////////////////////////////////////////////////////////////////////////////////////////
// MRDT 2019
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "RoveWatchdog.h"

#include "inc/hw_memmap.h"

#include "driverlib/sysctl.h"
#include "driverlib/watchdog.h"
#include "driverlib/rom_map.h"

//#define WATCHDOG0_BASE          0x40000000  // Watchdog0
//#define WATCHDOG1_BASE          0x40001000  // Watchdog1
// todo, beware of WATCHDOG0_BASE because we think it might conflict with Energia usage of SYSCTL_PERIPH_WTIMER0 ?
// Enable Watchdog Timer 1; supposedly timer 0 has a conflict in Energia, unconfirmed

// 5,500,000 => 6,000,000
// 500*120000000/1000 Why off by 10?

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//void RoveWatchdog::begin(void(*watchdogFunction)(void), unsigned int timeout_millis, unsigned int WatchdogTimerModule=WATCHDOG_1)

void RoveWatchdog::begin( void(*userFunction)(void), int timeout_millis, uint32_t watchdog )
{
  if(      watchdog     == WATCHDOG_0)
  {  this->watchdog_base = WATCHDOG0_BASE;
    SysCtlPeripheralEnable( SYSCTL_PERIPH_WDOG0 ); } 

  else if( watchdog_base == WATCHDOG_1)
  {
     this->watchdog_base = WATCHDOG1_BASE;
    SysCtlPeripheralEnable( SYSCTL_PERIPH_WDOG1 );
  }

  WatchdogUnlock(       this->watchdog_base );
  WatchdogReloadSet(    this->watchdog_base, (120000000 / 1000) * timeout_millis );
  WatchdogIntRegister(  this->watchdog_base, userFunction);
  WatchdogIntEnable(    this->watchdog_base);
  WatchdogResetDisable( this->watchdog_base );
  WatchdogEnable(       this->watchdog_base );
}

//////////////////////////////////

void RoveWatchdog::clear()
{
  WatchdogIntClear(this->watchdog_base);
}
