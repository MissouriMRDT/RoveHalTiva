# RoveHAL
Hardware Abstraction Layer for timer interrupt, pwm write, and pwm read for TM4C129 microcontroller based on Energia IDE platform
<pre>
// Be aware of PIN_CONFLICT => If you try to use the pin functions listed,
//                             Energia or RoveWare will steal this peripheral config from us,
//                             or we will steal the peripheral config from them,
//                             and the peripheral will only source whichever was config last

// The Energia::Servo module on the Tiva1294 is hardcoded to T2_A and will conflict with T2_A pin cfg.
// Energia::delay/millis/lwip/etc/etc all use systick callbacks so there are no conflicts with attachInterrupt Timers
// Energia::AnalogWrite or roveware::pwmRead will steal timers on the following pin by pin basis

const uint8_t INVALID =  0;
const uint8_t T0_A =  1; // PD_0, PA_0, PL_4 PIN_CONFLICTS Energia::AnalogWrite or roveware::pwmRead
const uint8_t T0_B =  2; // PD_1, PA_1, PL_5 PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
const uint8_t T1_A =  3; // PD_2, PA_2, PL_6 PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
const uint8_t T1_B =  4; // PD_3, PA_3, PL_7 PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
const uint8_t T2_A =  5; // PA_4, PM_0       PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead or Servo
const uint8_t T2_B =  6; // PA_5, PM_1       PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
const uint8_t T3_A =  7; // PA_6, PM_2, PD_4 PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
const uint8_t T3_B =  8; // PA_7, PM_3, PD_5 PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
const uint8_t T4_A =  9; // PM_4, PB_0, PD_6 PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
const uint8_t T4_B = 10; // PM_5, PB_1, PD_7 PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
const uint8_t T5_A = 11; // PM_6, PB_2       PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
const uint8_t T5_B = 12; // PM_7, PB_3       PIN_CONFLICTS Energia::AnalogWrite or RoveWare::pwmRead
const uint8_t T6_A = 13; // No conflicts? => todo ? Watchdog?
const uint8_t T6_B = 14; // No conflicts?
const uint8_t T7_A = 15; // No conflicts?
const uint8_t T7_B = 16; // No conflicts?
</pre>
