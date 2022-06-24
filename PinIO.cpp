/*
  name: PinIO.cpp
  catigory: pin control
*/

#include "Arduino.h"
#include "PinIO.h"

PinIO::PinIO(uint8_t pin) {
  _pin = pin;
  pinMode(_pin, 1);
  _state = 0;
};


bool PinIO::status() {
  return _state;
}


void PinIO::off() {
  _state = 0;
  digitalWrite(_pin, _state);
}


void PinIO::on() {
  _state = 1;
  digitalWrite(_pin, _state);
}


void PinIO::toggle() {
  _state = !_state;
  digitalWrite(_pin, _state);
}


void PinIO::write(bool value) {
  _value = value;
  digitalWrite(_pin, _value);
}





