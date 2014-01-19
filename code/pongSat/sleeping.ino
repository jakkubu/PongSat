// Wakeup pin based on:
// http://donalmorrissey.blogspot.com/2010/04/sleeping-arduino-part-5-wake-up-via.html
// http://citizen-sensing.org/2013/07/arduino-watchdog/


#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>



void go2sleep(int seconds){
  setupSleep();
  sleep_enable();
  for(int i=0;i<seconds;i++){
    if(!doSaveNow){
      sleep_mode();//One second sleep
    } else {
      Serial.println('button pressed');
      i = seconds;
    }
  }
  detachInterrupt (0);  
  sleep_disable();
    /* Re-enable the peripherals. */
  power_all_enable();
}

void setupSleep(){
/*** Setup the WDT ***/
  
  /* Clear the reset flag. */
  MCUSR &= ~(1<<WDRF);
  
  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  /* set new watchdog timeout prescaler value */
  WDTCSR = 1<<WDP1 | 1<<WDP2; /* ~1.0 seconds */
  
  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  doSaveNow = false;
  attachInterrupt(0, onButtonPressed, RISING);
}


ISR(WDT_vect){
/***WatchdogInterruptService - executed after each 1s sleep***/
//This is necessary even empty
}

void onButtonPressed(){
  doSaveNow = true;
  detachInterrupt (0);  
  sleep_disable();
}



