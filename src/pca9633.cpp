#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>
#include "pca9633.h"

PCA9633::PCA9633() {}

static void _i2c_wordwrite(uint8_t address, uint8_t cmd, uint16_t theWord) {
  //  Send output register address
  Wire.beginTransmission(address);
  Wire.write(cmd); // control register
  Wire.write(highByte(theWord));  //  high byte
  Wire.write(lowByte(theWord));  //  send low byte of word data
  Wire.endTransmission();
}

static void _i2c_write(uint8_t address, uint8_t cmd, uint8_t data) {
  //  Send output register address
  Wire.beginTransmission(address);
  Wire.write(cmd); // control register
  Wire.write(data);  //  send byte data
  Wire.endTransmission();
}

static uint16_t _i2c_wordread(uint8_t address, uint8_t cmd) {
  uint16_t result;
  uint8_t xlo, xhi;

  Wire.beginTransmission(address);
  Wire.write(cmd); // control register
  Wire.endTransmission();

  uint8_t readbytes = Wire.requestFrom(address, (uint8_t) 2); // request two bytes
  xhi = Wire.read();
  xlo = Wire.read();

  result = xhi << 8; // hi byte
  result = result | xlo; // add in the low byte

  return result;
}

static uint8_t _i2c_read(uint8_t address, uint8_t cmd) {
  uint8_t result = 0;
  uint8_t size = 1;
  uint8_t sendStop = true;

  Wire.beginTransmission(address);
  Wire.write(cmd); // control register
  Wire.endTransmission();

  // uglyness as a temporary fix for esp32 wire library which is different than esp8266 version
  #if defined(_ESP32) || defined(__AVR__)
  uint8_t readbytes = Wire.requestFrom(address, size, sendStop); // request cnt bytes
  #else
  uint8_t readbytes = Wire.requestFrom(address, (size_t)size, (bool)sendStop); // request cnt bytes
  #endif

  result  = Wire.read();

  return result;
}

uint8_t PCA9633::linearize(uint8_t pwm) {
  uint8_t result = pgm_read_byte(ledLinear + pwm);
  return result;
}

void PCA9633::chipinit(void) { // setup chip with desired operating parameters
  uint8_t m1 = 0x00; // set sleep = 0, turn on oscillator, disable allcall and subaddrs
  uint8_t m2 = ((INVRT) | (OUTDRV)); // output inverted, totem pole drivers enabled
  uint8_t ldout = 0xFF; // all outputs under individual and group control

  _i2c_write(_pcaAddr, MODE1, m1);
  _i2c_write(_pcaAddr, MODE2, m2);
  _i2c_write(_pcaAddr, LEDOUT, ldout);
}

void PCA9633::begin(uint8_t devAddr) { // just set device address
  _pcaAddr = devAddr;
  chipinit(); // setup chip
  setFade(10);
}

void PCA9633::begin(uint8_t devAddr, uint8_t fade_delay) { // set device address and fade delay
  _pcaAddr = devAddr;
  chipinit(); // setup chip
  setFade(fade_delay);
}

void PCA9633::begin(uint8_t devAddr, uint8_t fade_delay, uint8_t i2c_init) { // set device address, fade delay and init i2c using defaults
  _pcaAddr = devAddr;
  chipinit(); // setup chip
  setFade(fade_delay);
  if (i2c_init) { // start i2c bus
    Wire.begin();
  }
}

void PCA9633::setrgbw(uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3) {
  _i2c_write(_pcaAddr, PWM0, linearize(p0));
  _i2c_write(_pcaAddr, PWM1, linearize(p1));
  _i2c_write(_pcaAddr, PWM2, linearize(p2));
  _i2c_write(_pcaAddr, PWM3, linearize(p3));

}

void PCA9633::setFade(uint8_t dly) {
 _fadeDelay = dly;
 if (dly=0 || dly>100) _fadeDelay=10;
}

uint8_t PCA9633::getFade() {
  return _fadeDelay;
}

void PCA9633::setpwm(uint8_t pwmaddr, uint8_t pwmval) {
  uint8_t curval = _i2c_read(_pcaAddr, (pwmaddr + 2)); // read current value
  if (curval==pwmval) {
    // do nothing
  } else if (curval<pwmval) { // current value less than requested value
    for (int newval=curval+1; newval<=pwmval; newval++) { // add until equal
      if (newval>255) newval=255;
      _i2c_write(_pcaAddr, (pwmaddr + 2), newval);
      delay(_fadeDelay);
    }
   } else if (curval>pwmval) { // current value greater than requested value
    for (int newval=curval-1; newval>=pwmval; newval--) {
      if (newval<0) newval=0;
      _i2c_write(_pcaAddr, (pwmaddr + 2), newval);
      delay(_fadeDelay);
    }
  }
}

void PCA9633::setgrouppwm(uint8_t pwm) {
  _i2c_write(_pcaAddr, GRPPWM, pwm);
}
