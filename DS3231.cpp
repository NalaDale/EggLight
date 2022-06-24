#include "Arduino.h"
#include "DS3231.h"

#define REG_SEC		  0x00
#define REG_MIN		  0x01
#define REG_HOUR	  0x02
#define REG_DOW		  0x03
#define REG_DATE	  0x04
#define REG_MON		  0x05
#define REG_YEAR	  0x06
#define REG_CON		  0x0e
#define REG_STATUS  0x0f
#define REG_AGING	  0x10
#define REG_TEMPM	  0x11
#define REG_TEMPL	  0x12

#define SEC_1970_TO_2000 946684800

static const uint8_t dim[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/* Public */

Time::Time() {
  this->year = 2014;
  this->mon  = 1;
  this->date = 1;
  this->hour = 0;
  this->min  = 0;
  this->sec  = 0;
  this->dow  = 3;
}

DS3231::DS3231(uint8_t dataPin, uint8_t sclkPin) {
  _sdaPin = dataPin;
  _sclPin = sclkPin;
}

Time DS3231::getTime() {
  Time t;
  _burstRead();
  t.sec	= _decode(_burstArray[0]);
  t.min	= _decode(_burstArray[1]);
  t.hour	= _decodeH(_burstArray[2]);
  t.dow	= _burstArray[3];
  t.date	= _decode(_burstArray[4]);
  t.mon	= _decode(_burstArray[5]);
  t.year	= _decodeY(_burstArray[6]) + 2000;
  return t;
}

void DS3231::setTime(uint8_t hour, uint8_t min, uint8_t sec) {
  if ((hour < 24) && (min < 60) && (sec < 60)) {
    _writeRegister(REG_HOUR, _encode(hour));
    _writeRegister(REG_MIN, _encode(min));
    _writeRegister(REG_SEC, _encode(sec));
  }
}

void DS3231::setDate(uint8_t date, uint8_t mon, uint16_t year) {
  if (((date > 0) && (date <= 31)) && ((mon > 0) && (mon <= 12)) && ((year >= 2000) && (year < 3000))) {
    year -= 2000;
    _writeRegister(REG_YEAR, _encode(year));
    _writeRegister(REG_MON, _encode(mon));
    _writeRegister(REG_DATE, _encode(date));
  }
}

void DS3231::setDOW() {
  int dow;
  byte mArr[12] = {6, 2, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  Time _t = getTime();

  dow = (_t.year % 100);
  dow = dow * 1.25;
  dow += _t.date;
  dow += mArr[_t.mon - 1];
  if (((_t.year % 4) == 0) && (_t.mon < 3))
    dow -= 1;
  while (dow > 7)
    dow -= 7;
  _writeRegister(REG_DOW, dow);
}

void DS3231::setDOW(uint8_t dow) {
  if ((dow > 0) && (dow < 8))
    _writeRegister(REG_DOW, dow);
}


long DS3231::getUnixTime(Time t) {
  uint16_t	dc;

  dc = t.date;
  for (uint8_t i = 0; i < (t.mon - 1); i++)
    dc += dim[i];
  if ((t.mon > 2) && (((t.year - 2000) % 4) == 0))
    ++dc;
  dc = dc + (365 * (t.year - 2000)) + (((t.year - 2000) + 3) / 4) - 1;

  return ((((((dc * 24L) + t.hour) * 60) + t.min) * 60) + t.sec) + SEC_1970_TO_2000;
}



void DS3231::setOutput(byte enable) {
  uint8_t _reg = _readRegister(REG_CON);
  _reg &= ~(1 << 2);
  _reg |= (enable << 2);
  _writeRegister(REG_CON, _reg);
}


/* Private */

void	DS3231::_sendStart(byte addr) {
  pinMode(_sdaPin, OUTPUT);
  digitalWrite(_sdaPin, HIGH);
  digitalWrite(_sclPin, HIGH);
  digitalWrite(_sdaPin, LOW);
  digitalWrite(_sclPin, LOW);
  shiftOut(_sdaPin, _sclPin, MSBFIRST, addr);
}

void	DS3231::_sendStop() {
  pinMode(_sdaPin, OUTPUT);
  digitalWrite(_sdaPin, LOW);
  digitalWrite(_sclPin, HIGH);
  digitalWrite(_sdaPin, HIGH);
  pinMode(_sdaPin, INPUT);
}

void	DS3231::_sendNack() {
  pinMode(_sdaPin, OUTPUT);
  digitalWrite(_sclPin, LOW);
  digitalWrite(_sdaPin, HIGH);
  digitalWrite(_sclPin, HIGH);
  digitalWrite(_sclPin, LOW);
  pinMode(_sdaPin, INPUT);
}

void	DS3231::_sendAck() {
  pinMode(_sdaPin, OUTPUT);
  digitalWrite(_sclPin, LOW);
  digitalWrite(_sdaPin, LOW);
  digitalWrite(_sclPin, HIGH);
  digitalWrite(_sclPin, LOW);
  pinMode(_sdaPin, INPUT);
}

void	DS3231::_waitForAck() {
  pinMode(_sdaPin, INPUT);
  digitalWrite(_sclPin, HIGH);
  while (digitalRead(_sdaPin)); // pause while pin is high
  digitalWrite(_sclPin, LOW);
}

uint8_t DS3231::_readByte() {
  pinMode(_sdaPin, INPUT);

  uint8_t value = 0;
  uint8_t currentBit = 0;

  for (int i = 0; i < 8; ++i) {
    digitalWrite(_sclPin, HIGH);
    currentBit = digitalRead(_sdaPin);
    value |= (currentBit << (7 - i)); // (7-i) could be wrong
    delayMicroseconds(1);
    digitalWrite(_sclPin, LOW);
  }
  return value;
}

void DS3231::_writeByte(uint8_t value) {
  pinMode(_sdaPin, OUTPUT);
  shiftOut(_sdaPin, _sclPin, MSBFIRST, value);
}

uint8_t	DS3231::_decode(uint8_t value) {
  uint8_t decoded = value & 127;
  decoded = (decoded & 15) + 10 * ((decoded & (15 << 4)) >> 4);
  return decoded;
}

uint8_t DS3231::_decodeH(uint8_t value) {
  if (value & 128)
    value = (value & 15) + (12 * ((value & 32) >> 5));
  else
    value = (value & 15) + (10 * ((value & 48) >> 4));
  return value;
}

uint8_t	DS3231::_decodeY(uint8_t value) {
  uint8_t decoded = (value & 15) + 10 * ((value & (15 << 4)) >> 4);
  return decoded;
}

uint8_t DS3231::_encode(uint8_t value) {
  uint8_t encoded = ((value / 10) << 4) + (value % 10);
  return encoded;
}