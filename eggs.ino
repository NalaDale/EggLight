#include "settings.h"

#include "src/PinIO/PinIO.h"
PinIO relay(RELAY_PIN);

#include "src/DS3231/DS3231.h"
DS3231  rtc(A4, A5);
Time t;

int brightness;
bool relayState;

unsigned long previousMillis;

void setup()
{

#ifdef DEBUG_MODE
  Serial.begin(BAUD_RATE);
  Serial.println(F(""));
  Serial.println(F("ready to debug"));
#endif

  setPinModes();

  rtc.begin();

#ifdef SET_TIME
  // get each value
  String _Time = __TIME__;
  int hour = _Time.substring(0, 2).toInt();
  int min = _Time.substring(3, 5).toInt();
  int sec = _Time.substring(6, 8).toInt();

  // set rtc time
  rtc.setTime(hour, min, sec);  // (Hour, Minute, Second) in 24hr format

  // print to moniter
  char buffer[19];
  sprintf(buffer, "time set to %u:%u:%u", hour, min, sec);
  Serial.println(buffer);
#endif
}


void loop()
{

  if (millis() - previousMillis >= DELAY_TIME)
  {
    // save the last time you blinked the LED
    previousMillis = millis();



    t = rtc.getTime();

    brightness = map(analogRead(PHOTOCELL_PIN), 0, 1023, 1023, 0);
    // the map function inverts the state for internal pullup resistor


    updateRelay(); // uses "brightness" and "threshold"

#ifdef DEBUG_MODE
    printDebug();
#endif
  }
}
