#include <SD.h>
#include <ctype.h>

void parseConfigFile(File _myFile)
{
  char _character;
  String _description = "";  
    // read from the file until there's nothing else in it:
    while (_myFile.available()) {
      _character = _myFile.read();
      
             if(_character == '/')         {
               // Comment - ignore this line
               while(_character != '\n'){
                 _character = _myFile.read();
               };
      } else if(isalnum(_character))      {  // Add a _character to the _description
        _description.concat(_character);
      } else if(_character =='=')         {  // start checking the value for possible results
        //Serial.print("parsing: ");
        //Serial.println(_description);        
        parseValue(_description, _myFile);
        _description = "";
      } else {
        // Ignore this character (could be space, tab, newline, carriage return or something else)
      }
    }
}

void parseValue(String _description,File _myFile ){
  char _character;
  // First going to trim out all trailing white spaces

  do {
     _character = _myFile.read();
  } while(_character == ' ');
  if(_description == "mP") {
    measurePeriod = parseInteger(_myFile,_character,5000);
  } else if(_description == "SDWP") {
    SDwritingPeriod = parseInteger(_myFile,_character,10);
  } else if(_description == "iPM") {
    isPressureMeasured = parseBool(_character);
  } else if(_description == "iTM") {
    isTemperatureMeasured = parseBool(_character);
  } else if(_description == "dC") {
    datalogComment = parseString(_myFile,_character);
  //
  }else { // unknown parameter
    while(_character != '\n' && _myFile.available()){
      _character = _myFile.read();
    }
  }
}

bool parseBool(char _character){
  if (_character=='1') {
    return true;
  } else {
    return false;
  } 
}

String parseString(File _myFile, char _character){
  String _value = "";
  bool _bConcat = false;
  do {
    if (_character == '\"'){//detect " _character
      if(_bConcat){break;}
      _bConcat = true;
    } else if(_bConcat){
      _value.concat(_character);
    }
    _character = _myFile.read();
  } while(_character != '\n');
  //Serial.println(_value);
  return _value;
}

int parseInteger(File _myFile, char _character, int _defaultVal){
  char _value[5];
  int _counter =0;
    while(_character != '\n' && _character != '/' ) {
      // Serial.print(_character);
      if(isdigit(_character)) {
        _value[_counter] = _character;
        _counter++;
      } 
      _character = _myFile.read(); 
      
    };
      // Convert chars to integer
      //Serial.print("\n1st val = ");
      //Serial.println(String(atoi(_value)));
    if (_counter>0) {
      return atoi(_value);
      
          //  return atoi(charBuf);
    } else {
      // revert to default value for invalid entry in config
      Serial.println("integer not found!");
      return _defaultVal;
    }
}












