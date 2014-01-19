/*
 MPL115A1 SPI Digital Barometer Test Code
 Created on: September 30, 2010
       By: Jeremiah McConnell - miah at miah.com
 Portions: Jim Lindblom - jim at sparkfun.com
 
 This is a simple test program for the MPL115A1 Pressure Sensor (SPI version).
 
 Hardware: ATmega168, ATmega328
 Powered at 3.3V or 5V, running at 8MHz or 16MHz.
 
 MPL115A1 Breakout ------------- Arduino
 -----------------               -------
         SDN ------------------- D9
         CSN ------------------- D3 
         SDO ------------------- D12 *
         SDI ------------------- D11 *
         SCK ------------------- D13 *
         GND ------------------- GND
         VDD ------------------- VCC +
 
         * These pins physically connect to the SPI device so you can't change them
         + 5V board use 5V VDD, 3.3V board use 3.3V VDD
 
 License: CCAv3.0 Attribution-ShareAlike (http://creativecommons.org/licenses/by-sa/3.0/)
 You're free to use this code for any venture, but I'd love to hear about what you do with it,
 and any awesome changes you make to it. Attribution is greatly appreciated.
 */

void sensorSetup() {
    
    
    
    // initialize SPI interface
    SPI.begin();
    
    // these are the defaults
    //SPI.setDataMode(SPI_MODE0);
    //SPI.setClockDivider(SPI_CLOCK_DIV4);  // MPL115A1 supports up to 8MHz
    //SPI.setBitOrder(MSBFIRST);
    
    // initialize the chip select and enable pins
    pinMode(MPL115A1_SELECT_PIN, OUTPUT);
    pinMode(MPL115A1_ENABLE_PIN, OUTPUT);
    
    // sleep the MPL115A1
    digitalWrite(MPL115A1_ENABLE_PIN, LOW);
    
    // set the chip select inactive, select signal is CS LOW
    digitalWrite(MPL115A1_SELECT_PIN, HIGH);
    
    // spam welcome banner
//    Serial.print("\n************************************************************************************\n");
//    Serial.print("* Using Altimiter Setting: ");
//    Serial.print(NWS_BARO, 2);
//    Serial.print(" in Hg                                             *\n");
//    Serial.print("* Visit http://www.weather.gov for your current local value.                       *\n");
//    Serial.print("************************************************************************************\n");
}
 
 void doMeasurement(int measureNo) {
    
    float temperature_c= 0;
    float pressure_pKa = 0;
    //long altitude_ft = 0;
    
    // wake the MPL115A1
    digitalWrite(MPL115A1_ENABLE_PIN, HIGH);
    delay(20);  // give the chip a few ms to wake up

    if(isPressureMeasured){
      pressure_int[measureNo]  = calculatePressurekPa();
    }
    if(isTemperatureMeasured){
      temperature_int[measureNo] = calculateTemperatureC();
    }
    // pressure_pKa is defined as a floating point number
    // Conversion to decimal value from 1023 ADC count value
    // ADC counts are 0 to 1023, pressure is 50 to 115kPa respectively    
    pressure_pKa = ((65.0/1023.0)*pressure_int[measureNo])+50;    

    // -5.35 counts per Â°C, 472 counts is 25Â°C    
    temperature_c = 25 + (temperature_int[measureNo] - 472) / -5.35;
    // altitude_ft = calculateAltitudeFt(pressure_pKa);

    
    
    
    // put the MPL115A1 to sleep, it has this feature why not use it
    // while in shutdown the part draws ~1uA
    digitalWrite(MPL115A1_ENABLE_PIN, LOW);
    
    // print table of altitude, pressures, and temperatures to console
    Serial.print(pressure_pKa, 1);
    delay(20);
    Serial.print(" kPa | ");
    delay(20);
    
    // At a res of -5.35 counts/Â°C, digits lower than 0.1Â°C are not significant
    Serial.print(temperature_c, 1);
    delay(20);
    Serial.println(" C | ");
    delay(50);
    //Serial.print(DEGC_TO_DEGF(temperature_c), 1);
    //Serial.print(" F\n");
}
 




// long calculateAltitudeFt(float pressure_kPa) {
    
//     float delta;
//     long altitude_ft;
    
    
//     // See http://en.wikipedia.org/wiki/Barometric_formula
//     // If you're a pilot you need to know what's going on here,
//     // otherwise just know these steps calculate the barometric altitude (ft)
//     // based on the ratio of absolute barometric pressure (psia) to the
//     // altimiter setting (psia).
//     delta = KPA_TO_PSIA(pressure_kPa) / INHG_TO_PSIA( NWS_BARO );
//     altitude_ft = (1 - pow(delta, (1 / 5.25587611))) / 0.0000068756;
    
//     return altitude_ft;
// }


unsigned int  calculateTemperatureC() {
    
    unsigned int uiTadc; //temperature
    unsigned char uiTH, uiTL;
    
    unsigned int temperature_counts = 0;
    
    writeRegister(0x22, 0x00);  // Start temperature conversion
    delay(2);                   // Max wait time is 0.7ms, typ 0.6ms
    
    // Read pressure
    uiTH = readRegister(TEMPH);
    uiTL = readRegister(TEMPL);
    
    uiTadc = (unsigned int) uiTH << 8;
    uiTadc += (unsigned int) uiTL & 0x00FF;
    
    // Temperature is a 10bit value
    uiTadc = uiTadc >> 6;
    
    // -5.35 counts per Â°C, 472 counts is 25Â°C
    return uiTadc;
}


unsigned int calculatePressurekPa() {
    
    // See Freescale document AN3785 for detailed explanation
    // of this implementation.
    
    signed char sia0MSB, sia0LSB;
    signed char sib1MSB, sib1LSB;
    signed char sib2MSB, sib2LSB;
    signed char sic12MSB, sic12LSB;
    signed char sic11MSB, sic11LSB;
    signed char sic22MSB, sic22LSB;
    signed int sia0, sib1, sib2, sic12, sic11, sic22, siPcomp;
    signed long lt1, lt2, lt3, si_c11x1, si_a11, si_c12x2;
    signed long si_a1, si_c22x2, si_a2, si_a1x1, si_y1, si_a2x2;
    unsigned int uiPadc, uiTadc;
    unsigned char uiPH, uiPL, uiTH, uiTL;
    
    writeRegister(0x24, 0x00);      // Start Both Conversions
    //writeRegister(0x20, 0x00);    // Start Pressure Conversion
    //writeRegister(0x22, 0x00);    // Start temperature conversion
    delay(2);                       // Max wait time is 1ms, typ 0.8ms
    
    // Read pressure
    uiPH = readRegister(PRESH);
    uiPL = readRegister(PRESL);
    uiTH = readRegister(TEMPH);
    uiTL = readRegister(TEMPL);
    
    uiPadc = (unsigned int) uiPH << 8;
    uiPadc += (unsigned int) uiPL & 0x00FF;
    uiTadc = (unsigned int) uiTH << 8;
    uiTadc += (unsigned int) uiTL & 0x00FF;
    
    // Placing Coefficients into 16-bit Variables
    // a0
    sia0MSB = readRegister(A0MSB);
    sia0LSB = readRegister(A0LSB);
    sia0 = (signed int) sia0MSB << 8;
    sia0 += (signed int) sia0LSB & 0x00FF;
    
    // b1
    sib1MSB = readRegister(B1MSB);
    sib1LSB = readRegister(B1LSB);
    sib1 = (signed int) sib1MSB << 8;
    sib1 += (signed int) sib1LSB & 0x00FF;
    
    // b2
    sib2MSB = readRegister(B2MSB);
    sib2LSB = readRegister(B2LSB);
    sib2 = (signed int) sib2MSB << 8;
    sib2 += (signed int) sib2LSB & 0x00FF;
    
    // c12
    sic12MSB = readRegister(C12MSB);
    sic12LSB = readRegister(C12LSB);
    sic12 = (signed int) sic12MSB << 8;
    sic12 += (signed int) sic12LSB & 0x00FF;
    
    // c11
    sic11MSB = readRegister(C11MSB);
    sic11LSB = readRegister(C11LSB);
    sic11 = (signed int) sic11MSB << 8;
    sic11 += (signed int) sic11LSB & 0x00FF;
    
    // c22
    sic22MSB = readRegister(C22MSB);
    sic22LSB = readRegister(C22LSB);
    sic22 = (signed int) sic22MSB << 8;
    sic22 += (signed int) sic22LSB & 0x00FF;
    
    // Coefficient 9 equation compensation
    uiPadc = uiPadc >> 6;
    uiTadc = uiTadc >> 6;
    
    // Step 1 c11x1 = c11 * Padc
    lt1 = (signed long) sic11;
    lt2 = (signed long) uiPadc;
    lt3 = lt1*lt2;
    si_c11x1 = (signed long) lt3;
    
    // Step 2 a11 = b1 + c11x1
    lt1 = ((signed long)sib1)<<14;
    lt2 = (signed long) si_c11x1;
    lt3 = lt1 + lt2;
    si_a11 = (signed long)(lt3>>14);
    
    // Step 3 c12x2 = c12 * Tadc
    lt1 = (signed long) sic12;
    lt2 = (signed long) uiTadc;
    lt3 = lt1*lt2;
    si_c12x2 = (signed long)lt3;
    
    // Step 4 a1 = a11 + c12x2
    lt1 = ((signed long)si_a11<<11);
    lt2 = (signed long)si_c12x2;
    lt3 = lt1 + lt2;
    si_a1 = (signed long) lt3>>11;
    
    // Step 5 c22x2 = c22*Tadc
    lt1 = (signed long)sic22;
    lt2 = (signed long)uiTadc;
    lt3 = lt1 * lt2;
    si_c22x2 = (signed long)(lt3);
    
    // Step 6 a2 = b2 + c22x2
    lt1 = ((signed long)sib2<<15);
    lt2 = ((signed long)si_c22x2>1);
    lt3 = lt1+lt2;
    si_a2 = ((signed long)lt3>>16);
    
    // Step 7 a1x1 = a1 * Padc
    lt1 = (signed long)si_a1;
    lt2 = (signed long)uiPadc;
    lt3 = lt1*lt2;
    si_a1x1 = (signed long)(lt3);
    
    // Step 8 y1 = a0 + a1x1
    lt1 = ((signed long)sia0<<10);
    lt2 = (signed long)si_a1x1;
    lt3 = lt1+lt2;
    si_y1 = ((signed long)lt3>>10);
    
    // Step 9 a2x2 = a2 * Tadc
    lt1 = (signed long)si_a2;
    lt2 = (signed long)uiTadc;
    lt3 = lt1*lt2;
    si_a2x2 = (signed long)(lt3);
    
    // Step 10 pComp = y1 + a2x2
    lt1 = ((signed long)si_y1<<10);
    lt2 = (signed long)si_a2x2;
    lt3 = lt1+lt2;
    
    // Fixed point result with rounding
    //siPcomp = ((signed int)lt3>>13);
    siPcomp = lt3/8192;
    
    
    
    return siPcomp;
}


unsigned int readRegister(byte thisRegister) {
    
    byte result = 0;
    
    // select the MPL115A1
    digitalWrite(MPL115A1_SELECT_PIN, LOW);
    
    // send the request
    SPI.transfer(thisRegister | MPL115A1_READ_MASK);
    result = SPI.transfer(0x00);
    
    // deselect the MPL115A1
    digitalWrite(MPL115A1_SELECT_PIN, HIGH);
    
    return result;  
}


void writeRegister(byte thisRegister, byte thisValue) {
    
    // select the MPL115A1
    digitalWrite(MPL115A1_SELECT_PIN, LOW);
    
    // send the request
    SPI.transfer(thisRegister & MPL115A1_WRITE_MASK);
    SPI.transfer(thisValue);
    
    // deselect the MPL115A1
    digitalWrite(MPL115A1_SELECT_PIN, HIGH);
}

























