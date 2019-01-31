/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MRDT 2019 => TivaC1294/TivaC129E Pin Module Pulse Width Modulation Capture (PWM CCP)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "RovePwmRead.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Todo overflowing RoveRingBuff, tick reset

RovePwmRead    PwmRead_TimerT4APinPM4; // or TimerT4APinPB0 or TimerT4APinPD6

//////////////////
void debugPrint();

///////////////////////////////////////////////////////////////////////////////
void setup() 
{
  ////////////////////////////////////
  Serial.begin(9600);
  pinMode(PF_0, OUTPUT); // Todo debug
  pinMode(PF_4, OUTPUT); // Todo debug
  
  /////////////////////////////////////////////////////////////////////////////
  PwmRead_TimerT4APinPM4.attach( PM_4 ); // or attach( PB_0 ) or attach( PD_6 )
  PwmRead_TimerT4APinPM4.start();

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void debugPrint()
{
  Serial.println("////////////////////////////////////////////////////////");
  Serial.print("High PulseWidth Ticks:  "); Serial.println( PwmRead_TimerT4APinPM4.readHighWidthTicks()  );
  Serial.print("Low PulseWidth  Ticks:  "); Serial.println( PwmRead_TimerT4APinPM4.readLowWidthTicks()   );
  Serial.print("PulsePeriod     Ticks:  "); Serial.println( PwmRead_TimerT4APinPM4.readPeriodTicks()     );
  Serial.print("High PulseWidth Micros: "); Serial.println( PwmRead_TimerT4APinPM4.readHighWidthMicros() );
  Serial.print("Low PulseWidth  Micros: "); Serial.println( PwmRead_TimerT4APinPM4.readLowWidthMicros()  );
  Serial.print("PulsePeriod     Micros: "); Serial.println( PwmRead_TimerT4APinPM4.readPeriodMicros()    );
  Serial.print("Duty Cycle Decipercent: "); Serial.println( PwmRead_TimerT4APinPM4.readDutyDecipercent() );
  Serial.println();
}

///////////////
int duty = 0;
int high = 500;
int low  = 500;

//////////////////////////////////////////////////////
void loop() 
{
  debugPrint();
  delay(500);
  
  for( int i=0; i<1000; i++ )
  {
    duty = PwmRead_TimerT4APinPM4.readDutyDecipercent();
    if( duty < low  )  
    { low = duty;  }
    
    if( duty > high )  
    { high = duty; }
  }
  
  Serial.print("Duty:  "); Serial.println(  duty ); // Todo... unexpected values, debug capture conversion issues
  Serial.print("Low:  ");  Serial.println(  low  );
  Serial.print("High: ");  Serial.println(  high );
}

/*
/////////////////////////////////////////////////////////////////////////////////
RovePwmRead    PwmRead_TimerT0APinPD0; // or TimerT0APinPA0 or TimerT0APinPL4
RovePwmRead    PwmRead_TimerT0BPinPD1; // or TimerT0BPinPA1 or TimerT0BPinPL5
RovePwmRead    PwmRead_TimerT1APinPD2; // or TimerT1APinPA2 or TimerT1APinPL6
RovePwmRead    PwmRead_TimerT1BPinPD3; // or TimerT1BPinPA3 or TimerT1BPinPL7
RovePwmRead    PwmRead_TimerT2APinPA4; // or TimerT2APinPM0    
RovePwmRead    PwmRead_TimerT2BPinPA5; // or TimerT2BPinPM1    
RovePwmRead    PwmRead_TimerT3APinPA6; // or TimerT3APinPM2 or TimerT3APinPD4
RovePwmRead    PwmRead_TimerT3BPinPA7; // or TimerT3BPinPM3 or TimerT3BPinPD5
RovePwmRead    PwmRead_TimerT4APinPM4; // or TimerT4APinPB0 or TimerT4APinPD6
RovePwmRead    PwmRead_TimerT4BPinPM5; // or TimerT4BPinPB1 or TimerT4BPinPD7
RovePwmRead    PwmRead_TimerT5APinPM6; // or TimerT5APinPB2
RovePwmRead    PwmRead_TimerT5BPinPM7; // or TimerT5BPinPB3

///////////////////////////////////////////////////////////////
RovePwmReadWireBreaks     PwmReadWirebreaks_Timer6A;

//////////////////
void debugPrint();

///////////////////////////////////////////////////////////////////////////////
void setup() 
{
  ////////////////////////////////////
  Serial.begin(9600);
  pinMode(PF_0, OUTPUT); // Todo debug

  /////////////////////////////////////////////////////////////////////////////
  PwmRead_TimerT0APinPD0.attach( PD_0 ); // or attach( PA_0 ) or attach( PL_4 )
  PwmRead_TimerT0BPinPD1.attach( PD_1 ); // or attach( PA_1 ) or attach( PL_5 )
  PwmRead_TimerT1APinPD2.attach( PD_2 ); // or attach( PA_2 ) or attach( PL_6 )
  PwmRead_TimerT1BPinPD3.attach( PD_3 ); // or attach( PA_3 ) or attach( PL_7 )
  PwmRead_TimerT2APinPA4.attach( PA_4 ); // or attach( PM_0 )                 
  PwmRead_TimerT2BPinPA5.attach( PA_5 ); // or attach( PM_1 )                 
  PwmRead_TimerT3APinPA6.attach( PA_6 ); // or attach( PM_2 ) or attach( PD_4 )
  PwmRead_TimerT3BPinPA7.attach( PA_7 ); // or attach( PM_3 ) or attach( PD_5 )
  PwmRead_TimerT4APinPM4.attach( PM_4 ); // or attach( PB_0 ) or attach( PD_6 )
  PwmRead_TimerT4BPinPM5.attach( PM_5 ); // or attach( PB_1 ) or attach( PD_7 )
  PwmRead_TimerT5APinPM6.attach( PM_6 ); // or attach( PB_2 )
  PwmRead_TimerT5BPinPM7.attach( PM_7 ); // or attach( PB_3 )

  PwmReadWirebreaks_Timer6A.attachMicros( T6_A, 100 );
  PwmReadWirebreaks_Timer6A.start();

  PwmRead_TimerT0APinPD0.start();
  PwmRead_TimerT0BPinPD1.start();
  PwmRead_TimerT1APinPD2.start();
  PwmRead_TimerT1BPinPD3.start();
  PwmRead_TimerT2APinPA4.start();
  PwmRead_TimerT2BPinPA5.start();
  PwmRead_TimerT3APinPA6.start();
  PwmRead_TimerT3BPinPA7.start();
  PwmRead_TimerT4APinPM4.start();
  PwmRead_TimerT4BPinPM5.start();
  PwmRead_TimerT5APinPM6.start();
  PwmRead_TimerT5BPinPM7.start();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void debugPrint()
{
  Serial.print( "PwmRead_TimerT0APinPD0 width  ticks: ");  Serial.print(   PwmRead_TimerT0APinPD0.readWidthTicks());
  Serial.print( " PwmRead_TimerT0APinPD0 period ticks: "); Serial.print(   PwmRead_TimerT0APinPD0.readPeriodTicks());
  Serial.print( ", wirebreak: ");
  if( PwmRead_TimerT0APinPD0.isWireBroken() ) { Serial.println("is broken");     }
  else                                        { Serial.println("is NOT broken"); }

  Serial.print( "PwmRead_TimerT0BPinPD1 width  ticks: ");  Serial.print(   PwmRead_TimerT0BPinPD1.readWidthTicks());
  Serial.print( " PwmRead_TimerT0BPinPD1 period ticks: "); Serial.print(   PwmRead_TimerT0BPinPD1.readPeriodTicks()); 
  Serial.print( ", wirebreak: ");
  if( PwmRead_TimerT0BPinPD1.isWireBroken() ) { Serial.println("is broken");     }
  else                                        { Serial.println("is NOT broken"); }

  Serial.print( "PwmRead_TimerT1APinPD2 width  ticks: ");  Serial.print(   PwmRead_TimerT1APinPD2.readWidthTicks());
  Serial.print( " PwmRead_TimerT1APinPD2 period ticks: "); Serial.print(   PwmRead_TimerT1APinPD2.readPeriodTicks());  
  Serial.print( ", wirebreak: ");
  if( PwmRead_TimerT1APinPD2.isWireBroken() ) { Serial.println("is broken");     }
  else                                        { Serial.println("is NOT broken"); }

  Serial.print( "PwmRead_TimerT1BPinPD3 width  ticks: ");  Serial.print(   PwmRead_TimerT1BPinPD3.readWidthTicks()); 
  Serial.print( " PwmRead_TimerT1BPinPD3 period ticks: "); Serial.print(   PwmRead_TimerT1BPinPD3.readPeriodTicks()); 
  Serial.print( ", wirebreak: ");
  if( PwmRead_TimerT1BPinPD3.isWireBroken() ) { Serial.println("is broken");     }
  else                                        { Serial.println("is NOT broken"); }

  Serial.print( "PwmRead_TimerT2APinPA4 width  ticks: ");  Serial.print(   PwmRead_TimerT2APinPA4.readWidthTicks()); 
  Serial.print( " PwmRead_TimerT2APinPA4 period ticks: "); Serial.print(   PwmRead_TimerT2APinPA4.readPeriodTicks()); 
  Serial.print( ", wirebreak: ");
  if( PwmRead_TimerT2APinPA4.isWireBroken() ) { Serial.println("is broken");     }
  else                                        { Serial.println("is NOT broken"); }

  Serial.print( "PwmRead_TimerT2BPinPA5 width  ticks: ");  Serial.print(   PwmRead_TimerT2BPinPA5.readWidthTicks());
  Serial.print( " PwmRead_TimerT2BPinPA5 period ticks: "); Serial.print(   PwmRead_TimerT2BPinPA5.readPeriodTicks()); 
  Serial.print( ", wirebreak: ");
  if( PwmRead_TimerT2BPinPA5.isWireBroken() ) { Serial.println("is broken");     }
  else                                        { Serial.println("is NOT broken"); }

  Serial.print( "PwmRead_TimerT3APinPA6 width  ticks: ");  Serial.print(   PwmRead_TimerT3APinPA6.readWidthTicks());
  Serial.print( " PwmRead_TimerT3APinPA6 period ticks: "); Serial.print(   PwmRead_TimerT3APinPA6.readPeriodTicks());  
  Serial.print( ", wirebreak: ");
  if( PwmRead_TimerT3APinPA6.isWireBroken() ) { Serial.println("is broken");     }
  else                                        { Serial.println("is NOT broken"); }
  
  Serial.print( "PwmRead_TimerT3BPinPA7 width  ticks: ");  Serial.print(   PwmRead_TimerT3BPinPA7.readWidthTicks());
  Serial.print( " PwmRead_TimerT3BPinPA7 period ticks: "); Serial.print(   PwmRead_TimerT3BPinPA7.readPeriodTicks()); 
  Serial.print( ", wirebreak: ");
  if( PwmRead_TimerT3BPinPA7.isWireBroken() ) { Serial.println("is broken");     }
  else                                        { Serial.println("is NOT broken"); }

  Serial.print( "PwmRead_TimerT4APinPM4 width  ticks: ");  Serial.print(   PwmRead_TimerT4APinPM4.readWidthTicks());
  Serial.print( " PwmRead_TimerT4APinPM4 period ticks: "); Serial.print(   PwmRead_TimerT4APinPM4.readPeriodTicks()); 
  Serial.print( ", wirebreak: ");
  if( PwmRead_TimerT4APinPM4.isWireBroken() ) { Serial.println("is broken");     }
  else                                        { Serial.println("is NOT broken"); }

  Serial.print( "PwmRead_TimerT4BPinPM5 width  ticks: ");  Serial.print(   PwmRead_TimerT4BPinPM5.readWidthTicks());
  Serial.print( " PwmRead_TimerT4BPinPM5 period ticks: "); Serial.print(   PwmRead_TimerT4BPinPM5.readPeriodTicks()); 
  Serial.print( ", wirebreak: ");
  if( PwmRead_TimerT4BPinPM5.isWireBroken() ) { Serial.println("is broken");     }
  else                                        { Serial.println("is NOT broken"); }

  Serial.print( "PwmRead_TimerT5APinPM6 width  ticks: ");  Serial.print(   PwmRead_TimerT5APinPM6.readWidthTicks());
  Serial.print( " PwmRead_TimerT5APinPM6 period ticks: "); Serial.print(   PwmRead_TimerT5APinPM6.readPeriodTicks()); 
  Serial.print( ", wirebreak: ");
  if( PwmRead_TimerT5APinPM6.isWireBroken() ) { Serial.println("is broken");     }
  else                                        { Serial.println("is NOT broken"); }

  Serial.print( "PwmRead_TimerT5BPinPM7 width  ticks: ");  Serial.print(   PwmRead_TimerT5BPinPM7.readWidthTicks());
  Serial.print( " PwmRead_TimerT5BPinPM7 period ticks: "); Serial.print(   PwmRead_TimerT5BPinPM7.readPeriodTicks()); 
  Serial.print( ", wirebreak: ");
  if( PwmRead_TimerT5BPinPM7.isWireBroken() ) { Serial.println("is broken");     }
  else                                        { Serial.println("is NOT broken"); }
}

//////////////////////////////////////////////////////
void loop() 
{
  Serial.println("RovePwmRead all"); Serial.println();

  //////////////////////////////
  PwmRead_TimerT0APinPD0.stop(); 
  PwmRead_TimerT0BPinPD1.stop(); 
  PwmRead_TimerT1APinPD2.stop(); 
  PwmRead_TimerT1BPinPD3.stop(); 
  PwmRead_TimerT2APinPA4.stop(); 
  PwmRead_TimerT2BPinPA5.stop(); 
  PwmRead_TimerT3APinPA6.stop(); 
  PwmRead_TimerT3BPinPA7.stop(); 
  PwmRead_TimerT4APinPM4.stop(); 
  PwmRead_TimerT4BPinPM5.stop(); 
  PwmRead_TimerT5APinPM6.stop(); 
  PwmRead_TimerT5BPinPM7.stop(); 

  debugPrint();
  delay(10); 

  debugPrint();
  delay(10);

  ///////////////////////////////
  PwmRead_TimerT0APinPD0.start(); 
  PwmRead_TimerT0BPinPD1.start(); 
  PwmRead_TimerT1APinPD2.start(); 
  PwmRead_TimerT1BPinPD3.start(); 
  PwmRead_TimerT2APinPA4.start(); 
  PwmRead_TimerT2BPinPA5.start(); 
  PwmRead_TimerT3APinPA6.start(); 
  PwmRead_TimerT3BPinPA7.start(); 
  PwmRead_TimerT4APinPM4.start(); 
  PwmRead_TimerT4BPinPM5.start(); 
  PwmRead_TimerT5APinPM6.start(); 
  PwmRead_TimerT5BPinPM7.start(); 

  debugPrint();
  delay(10); 

  debugPrint();
  delay(10);
}
*/
