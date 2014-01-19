#include <SD.h>
//#include <MemoryFree.h>
#include <SPI.h>


#include "pongSat.h"



void setup(){
   // initialize serial  i/o
  // TODO sprawdz czy tu nie powinno bc SPI z pressure sensor
  Serial.begin(9600);
  sensorSetup();
  SDsetup();  
}


void loop() {
  int measureNo;
  if (!doSaveNow){
    for(measureNo=0;measureNo<SDwritingPeriod;measureNo++){
      doMeasurement(measureNo);
      //measurements are automatically saved in pressure_int and temperature_int
      if (!doSaveNow){
        go2sleep(measurePeriod);
      }else{
        break;
      }
    }
    Serial.println("writing datalog...");
    if (doSaveNow){
      Serial.println("End of measurement");
    }
    writeData2SD(measureNo);
    blinkDiode(); 
  }
  blinkDiode(); 
  delay(100);
}

void blinkDiode(){
  digitalWrite(MPL115A1_ENABLE_PIN, HIGH);//MPL115A1_ENABLE_PIN has attached led 
  delay(50);
  digitalWrite(MPL115A1_ENABLE_PIN, LOW);
  delay(50);
  digitalWrite(MPL115A1_ENABLE_PIN, HIGH);
  delay(50);
  digitalWrite(MPL115A1_ENABLE_PIN, LOW);
}
