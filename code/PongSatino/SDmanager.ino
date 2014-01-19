

void SDsetup(){
  Serial.println("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  do {
    isSdInitilized = initSD();
    if (isSdInitilized){
      // read settings
      initSettings();
      readConfigFile();
      initDatalogFile();
    } else {
      blinkDiode();
      delay(50);
      blinkDiode();
    }
  } while(!isSdInitilized);
}

void initDatalogFile(){
  File _dataFile = SD.open("DATALOG.TXT", FILE_WRITE);

  // if the file is available, write to it:
  if (_dataFile) {
    _dataFile.println("\n\nNEW MEASUREMENT");
    _dataFile.flush();
    _dataFile.println(datalogComment);
    _dataFile.flush();
    _dataFile.println("\nmP\tiPM\tiTM\tSDWP\t");
    _dataFile.flush();
    _dataFile.print(String(measurePeriod) +'\t' + String(isPressureMeasured) +'\t');
    _dataFile.flush();
    _dataFile.println(String(isTemperatureMeasured) +'\t' + String(SDwritingPeriod)+ '\n');
    _dataFile.flush();
    _dataFile.println("p\tt");   
    _dataFile.close();
    // print to the serial port too:
    //Serial.println(_dataString);
  }  
  // if the file isn't open, pop up an error:
  else {
    //Serial.println("error opening DATALOG.TXT");
  } 
  _dataFile.close();
}

void initSettings(){
  measurePeriod = 1000;
  SDwritingPeriod = 10;
  isPressureMeasured = true;
  isTemperatureMeasured = false;
  datalogComment = "default config";
}

boolean initSD(){
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return false;
  } else {
    //Serial.println("card initialized.");
    return true;
  }
}

void readConfigFile(){
  File _configFile;
  if (SD.exists("CONFIG.TXT")){
      _configFile = SD.open("CONFIG.TXT");
      //Serial.println("CONFIG.TXT exist");
      if (_configFile){
        parseConfigFile(_configFile);
      }      
    } 
  _configFile.close();
  delay(100);
  //Serial.println("closing configFile");
}

void writeData2SD(int numData2Save){
  //save to file only if anything is measured
  if(isTemperatureMeasured||isPressureMeasured){
    String _dataString;
    File _dataFile = SD.open("DATALOG.TXT", FILE_WRITE);

    // if the file is available, write to it:
    if (_dataFile) {
      for (int i=0;i<numData2Save;i++){
        _dataString="";
        if(isPressureMeasured){
          _dataString = String(pressure_int[i]);
        }
        _dataString += "\t";
        if(isTemperatureMeasured){
          _dataString += String(temperature_int[i]);
        }
        _dataFile.println(_dataString);
      }
      _dataFile.println("");
      _dataFile.close();
      // print to the serial port too:
      //Serial.println(_dataString);
    }  
    // if the file isn't open, pop up an error:
    else {
      //Serial.println("error opening DATALOG.TXT");
    } 
    _dataFile.close();
  }
}





// void readDataLog(){
  //   //logDebug("ps_datalog - readDataLog");
  //   File myFile = SD.open("DATALOG.TXT");
  //   Serial.println("opening myFile");
  //   if (myFile) {
  //     //Serial.println("DATALOG.TXT:");
      
  //     // read from the file until there's nothing else in it:
  //     while (myFile.available()) {
  //       Serial.write(myFile.read());
  //     }
  //   } else {
  //     // if the file didn't open, print an error:
  //     //Serial.println("error opening DATALOG.TXT");
  //   }
  //     // close the file:
  //   myFile.close();
  //   delay(100);
  //   Serial.println("closing myFile");
  // }






// void parseMeasureNo(){
  //   int num;
  //   File numFile;
  //   if (SD.exists("LOGNUM.TXT")){
  //     numFile = SD.open("LOGNUM.TXT");
  //     Serial.println("opening numFile");
  //     if(numFile){
  //       num = parseInteger(numFile, '#',0);
  //     } else{
  //       //Serial.println("unable to open ps_measure_num.txt file");
  //       num = 0;
  //     }
  //     numFile.close();
  //     delay(100);
  //     Serial.println("closing numFile");
  //   } else {
  //     num = 0;
  //   }
  //   delay(20);
  //   SD.remove("LOGNUM.TXT");
  //   delay(20);
  //   // createNumFile num+1;
  //     numFile = SD.open("LOGNUM.TXT",FILE_WRITE);
  //     Serial.println("opening numFile");
  //       if(numFile){
  //         numFile.print("#");
  //         numFile.print(String(num+1));
  //       }else{
  //         //Serial.println("unable to (re)create ps_measure_num.txt file");
  //       }
  //     numFile.close();
  //     delay(100);
  //     Serial.println("closing numFile");
  //   config.measuremenNum = num;
// }






