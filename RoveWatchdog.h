/////////////////////////////////////////////////////////////////////////////////////////////////////
// MRDT 2019
/////////////////////////////////////////////////////////////////////////////////////////////////////

// Todo

#ifndef ROVE_WATCHDOG_H
#define ROVE_WATCHDOG_H

#include <stdint.h>

#define WATCHDOG_0    0x0
#define WATCHDOG_1    0x1

class RoveWatchdog
{
public:

  void begin( void(*userFunction)(void), int timeout_millis, uint32_t watchdog=WATCHDOG_1 ); 
  void clear();

private:
   uint32_t watchdog_base;
};

#endif // ROVE_WATCHDOG_H