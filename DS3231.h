#ifndef DS3231_h
#define DS3231_h

#include "Arduino.h"

#define DS3231_ADDR_R	0xD1
#define DS3231_ADDR_W	0xD0
#define DS3231_ADDR		0x68

#define FORMAT_SHORT	1
#define FORMAT_LONG		2

#define FORMAT_LITTLEENDIAN	1
#define FORMAT_BIGENDIAN   	2
#define FORMAT_MIDDLEENDIAN	3

#define MONDAY		1
#define TUESDAY		2
#define WEDNESDAY	3
#define THURSDAY	4
#define FRIDAY		5
#define SATURDAY	6
#define SUNDAY		7

#define OUTPUT_INT		1

class Time {
  public:
    Time();
    uint8_t		hour;
    uint8_t		min;
    uint8_t		sec;
    uint8_t		date;
    uint8_t		mon;
    uint16_t	year;
    uint8_t		dow;
};


class DS3231 {
  public:
    DS3231(uint8_t data_pin, uint8_t sclk_pin);
    void	begin();
    Time	getTime();
    void	setTime(uint8_t hour, uint8_t min, uint8_t sec);
    void	setDate(uint8_t date, uint8_t mon, uint16_t year);
    void	setDOW();
    void	setDOW(uint8_t dow);
    long	getUnixTime(Time t);
    void	setOutput(byte enable);

  private:
    uint8_t _sclPin;
    uint8_t _sdaPin;
    uint8_t _burstArray[7];
    bool	_useHw;

    void	_sendStart(byte addr);
    void	_sendStop();
    void	_sendAck();
    void	_sendNack();
    void	_waitForAck();
    uint8_t	_readByte();
    void	_writeByte(uint8_t value);
    void	_burstRead();
    uint8_t	_readRegister(uint8_t reg);
    void 	_writeRegister(uint8_t reg, uint8_t value);
    uint8_t	_decode(uint8_t value);
    uint8_t	_decodeH(uint8_t value);
    uint8_t	_decodeY(uint8_t value);
    uint8_t	_encode(uint8_t vaule);

};
#endif