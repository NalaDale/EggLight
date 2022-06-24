//#define SET_TIME // if necessary, uncomment this line to set the time, upload, recomment, and upload again
#define DEBUG_MODE // for debugging

// settings
#ifdef DEBUG_MODE
  #define RELAY_PIN 13
  #define BAUD_RATE 9600
  #define DELAY_TIME 500
  #define THRESHOLD 500 
#else
  #define RELAY_PIN 4
  #define DELAY_TIME 600000 // 10 min
  #define THRESHOLD 500  
#endif

#define PHOTOCELL_PIN A0

#define ON_HOUR 6
#define OFF_HOUR 20 // 24 hr. time
