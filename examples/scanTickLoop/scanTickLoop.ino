///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MRDT 2019 => Tiva C 1294/129E Launchpad Timer Interrupt Module (Timer ISR)
// Todo      => Document Pin Conflicts on Serial/Servo/etc, debug timer_channel_b (even ROVE_TIMERS are failing)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "RoveTimerInterrupt.h"

RoveTimerInterrupt ScanTimer( T0_A );

volatile bool scan_blink         = LOW;  

void scan()
{
      scan_blink = (!scan_blink);
  digitalWrite(PF_0, scan_blink); //delayMicroseconds(10); // simulate some blocking work
}
void setup() {

  pinMode(PN_1, OUTPUT); // 1294 Launchpad Led D1
  pinMode(PN_0, OUTPUT); // 1294 Launchpad Led D2
  pinMode(PF_4, OUTPUT); // 1294 Launchpad Led D3
  pinMode(PF_0, OUTPUT); // 1294 Launchpad Led D4
  
  ScanTimer.attach( scan, 1000 );
  ScanTimer.start();
}

void loop() {
  
}















/*
// int period_micros   = 1;         // ~1Mhz max frequency
// int period_micros   = 139810;    //  max period on SYS_CLOCK source
// int period_micros   = 139811;    //  switch to PIOSC source above max period on SYS_CLOCK source
// int period_micros   = 1048000;   //  max period in microseconds (~1hz min frequency)
// int period_micros   = 1049000;   //  returns zero on user exceed max period microseconds
// 7 low ~ 0 high pri

#include "roveAttachTimerInterrupt.h"

////////////
void scan();
void tick();
void isr_a();
void isr_b();

/////////////////////////////////////////////////////////////////////////////////////////
volatile bool scan_blink         = LOW;  // blink state toggle
          int scan_priority      = 7;    // lowest priority
          int scan_period_millis = 1048; // 1hZ using min freq

void scan()
{
      scan_blink = (!scan_blink);
  digitalWrite(PF_0, scan_blink); //delayMicroseconds(10); // simulate some blocking work
}

///////////////////////////////////////////////////////////
volatile bool tick_blink = LOW;
         int  tick_priority      = 7;
         int  tick_period_millis = 1048;

void tick()
{
      tick_blink = (!tick_blink);
  digitalWrite(PF_4, tick_blink); //delayMicroseconds(10);
}

///////////////////////////////////////////////////////////
volatile bool isr_a_blink         = LOW;
          int isr_a_priority      = 7;
          int isr_a_period_millis = 1048;
          
void isr_a()
{
     isr_a_blink = (!isr_a_blink);
  digitalWrite(PN_0, isr_a_blink); //delayMicroseconds(10);
}

///////////////////////////////////////////////////////////
volatile bool isr_b_blink         = LOW;
          int isr_b_priority      = 7;
          int isr_b_period_millis = 1048;
          
void isr_b()
{
     isr_b_blink = (!isr_b_blink);
  digitalWrite(PN_1, isr_b_blink); //delayMicroseconds(10);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  pinMode(PN_1, OUTPUT); // 1294 Launchpad Led D1
  pinMode(PN_0, OUTPUT); // 1294 Launchpad Led D2
  pinMode(PF_4, OUTPUT); // 1294 Launchpad Led D3
  pinMode(PF_0, OUTPUT); // 1294 Launchpad Led D4
  roveAttachTimerInterrupt(        scan,  T0_A, scan_period_millis/30,        scan_priority ); //   30Hz speed on piosc visible to the eye
//roveAttachTimerInterruptMicros(  scan,  T0_A, scan_period_millis/1000000,   scan_priority ); //   1MHz speed on sysclock 
//roveAttachTimerInterruptTicks24( scan,  T0_A, TIMER_INTERRUPT_USE_PISOC,    30, 7 );         // 533kHz  = 1.8 micros at 30 pisoc    ticks at 62.5 nanos per tick
//roveAttachTimerInterruptTicks24( scan,  T0_A, TIMER_INTERRUPT_USE_SYSCLOCK, 30, 0 );         //    4MHz = 250 nanos  at 30 sysclock ticks at  8.3 nanos per tick
  roveAttachTimerInterrupt(        tick,  T1_B, tick_period_millis/15,        tick_priority );
  roveAttachTimerInterrupt(        isr_a, T7_A, isr_a_period_millis/5,        isr_a_priority );
  roveAttachTimerInterrupt(        isr_b, T7_B, isr_b_period_millis/2,        isr_b_priority );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void  loop()
{
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 8 bit prescaler on the 16b half timer ab in up count mode provides twelve 0xFF.xFFFF 24b timers T0_A ~ T5_B
  // 2^24 => 16,777,216 ticks on 120,000,000 tick/sec SYSCLOCK or 16,000,000 tick/per sec POISC => 1micro ~ 1.04sec range 
  // using => tm4c1294ncpdt.pdf => Table 13-5. 16-Bit Timer With Prescaler Configurations
  
  Serial.println("//////////////////////////////////////////////////////////////////////");
  Serial.println("TivaC 24b half Timers with prescaler and dual clock source T0_A ~ T5_B");
  Serial.println("                          => 1000mS ~ 1uS == 1Hz ~ 1MHz");
  Serial.println();
  Serial.println("//////////////////////////////////////////////////////////////////////");
  Serial.print("HiRes 120Mhz clock source => SYSCLOCK_TICKS_PER_MICRO       : "); Serial.println(                ( 120000000 / 1000000 )  );
  Serial.print("               139 millis => SYSCLOCK_MICROS_PER_MAX_PERIOD : "); Serial.println( ( 0x00FFFFFF / ( 120000000 / 1000000 ) )); 
  Serial.print("LowRes 16Mhz clock source => PIOSC_TICKS_PER_MICRO          : "); Serial.println(                (  16000000 / 1000000 )  );
  Serial.print("             1.048 sec    => PIOSC_MICROS_PER_MAX_PERIOD    : "); Serial.println( ( 0x00FFFFFF / (  16000000 / 1000000 ) ));
  Serial.println();
  
  delay(2000); // simulate some blocking
  roveStopTimerInterrupt(T0_A);
  roveStopTimerInterrupt(T1_B);
  roveStopTimerInterrupt(T7_A);
  roveStopTimerInterrupt(T7_B);

  digitalWrite(PN_1, LOW); // 1294 Launchpad Led D1
  digitalWrite(PN_0, LOW); // 1294 Launchpad Led D2
  digitalWrite(PF_4, LOW); // 1294 Launchpad Led D3
  digitalWrite(PF_0, LOW); // 1294 Launchpad Led D4
        
  delay(2000); // simulate some blocking
  roveRestartTimerInterrupt(T0_A);
  roveRestartTimerInterrupt(T1_B);
  roveRestartTimerInterrupt(T7_A);
  roveRestartTimerInterrupt(T7_B); 
}
*/