#include "inc/tm4c1294ncpdt.h"
#include "inc/hw_gpio.h"
#include "roveAttachTimerInterrupt.h"

int numRepeats = 100000;
int count1 = 0;
uint8_t bus_value = 0;

void writeClock();

/*This code tests how long it takes to run writeClock()
void setup()
{
  Serial.begin(115200);
}

void loop()
{
  count1 = 0;
  int firstTime = micros();
  while(count1<numRepeats)
  {
     writeClock();
  }
  int secondTime = micros();
  Serial.println((secondTime-firstTime)*1000/numRepeats);
}
  250ns
*/

/*This code tests various values for AttachTimerInterrupt
void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("init");

  // start the SPI library:
  pinMode(PE_0, OUTPUT);
  pinMode(PE_1, OUTPUT);
  pinMode(PE_2, OUTPUT);
  pinMode(PE_3, OUTPUT);

  digitalWrite(PE_0, HIGH);
  digitalWrite(PE_1, HIGH);
  digitalWrite(PE_2, HIGH);
  digitalWrite(PE_3, HIGH);

  //roveAttachTimerInterruptTicks24( writeClock,  T0_A, TIMER_INTERRUPT_USE_PISOC,    30, 7 );
  roveAttachTimerInterruptTicks24( writeClock,  T0_A, TIMER_INTERRUPT_USE_SYSCLOCK, 40, 0 ); 
  
  delay(100);
}
void loop() {
  
  int firstTime = micros();
  while(count1 <= numRepeats)
  {
    delay(count1/10000000);
  }
  int secondTime = micros();
  count1 = 0;
  Serial.print(secondTime-firstTime);
  Serial.print(":");
  Serial.print((secondTime-firstTime)*1000/numRepeats);
  
  Serial.println("ns");
}*/

void writeClock()
{
  HWREG(GPIO_PORTE_BASE + (B00001111 << 2)) = (count1&B00001001);
  count1 ++;
}

/* Test Results
 *  1000=8340ns
 *  100 =1681ns
 *  50  =1257ns
 *  45  =1141ns
 *  Cannot get reading below 45
 */
