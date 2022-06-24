/*
  name: PinIO.h
  catigory: pin control
*/

#ifndef PIN_IO_h
#define PIN_IO_h

#include "Arduino.h"

class PinIO {
  public:
    PinIO(uint8_t pin);
    bool status();
    void off();
    void on();
    void toggle();
    void write(bool value);

  private:
    bool _state;
    bool _value;
    uint8_t _pin;
};
#endif