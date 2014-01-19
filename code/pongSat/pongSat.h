// Includes


// Get your current altimiter setting from from the National
// Weather Service - http://www.weather.gov
// This value is often labeled "Barometer" or "Barometric Pressure" or just "Pressure"


#define NWS_BARO 29.65 

// Pin definitions
#define MPL115A1_ENABLE_PIN 9
#define MPL115A1_SELECT_PIN 3

// 3 for MPL115A1 SPI i/o
#define MPL115A1_READ_MASK  0x80
#define MPL115A1_WRITE_MASK 0x7F 

// MPL115A1 register address map
#define PRESH   0x00    // 80
#define PRESL   0x02    // 82
#define TEMPH   0x04    // 84
#define TEMPL   0x06    // 86

#define A0MSB   0x08    // 88
#define A0LSB   0x0A    // 8A
#define B1MSB   0x0C    // 8C
#define B1LSB   0x0E    // 8E
#define B2MSB   0x10    // 90
#define B2LSB   0x12    // 92
#define C12MSB  0x14    // 94
#define C12LSB  0x16    // 96
#define C11MSB  0x18    // 98
#define C11LSB  0x1A    // 9A
#define C22MSB  0x1C    // 9C
#define C22LSB  0x1E    // 9E

// Unit conversion macros
#define FT_TO_M(x) ((long)((x)*(0.3048)))
#define KPA_TO_INHG(x) ((x)*(0.295333727))
#define KPA_TO_MMHG(x) ((x)*(7.50061683))
#define KPA_TO_PSIA(x) ((x)*(0.145037738))
#define KPA_TO_KGCM2(x) ((x)*(0.0102))
#define INHG_TO_PSIA(x) ((x)*(0.49109778))
#define DEGC_TO_DEGF(x) ((x)*(9.0/5.0)+32)

//String dataLogPreString = "ps_datalog_";
//char dataLogFileName[100] = "ps_datalog.txt";
//unsigned int pressure_int = 0;
//unsigned int temperature_int = 0;

unsigned int pressure_int[100] ;
unsigned int temperature_int[100] ;


//--------------PARAMETERS------------------

int measurePeriod;
int SDwritingPeriod;
bool isPressureMeasured;
bool isTemperatureMeasured;
String datalogComment;


//--------------SD CARD---------------------
const int chipSelect = 10;
bool isSdInitilized  = false;

//--------------SLEEPING---------------------

bool doSaveNow = false;


