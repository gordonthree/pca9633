// include library
#include <pca9633.h>
#include <Wire.h> // OPTIONAL see below under setup()

// create instance
PCA9633 rgbw; 

void setup() {
#if defined(__AVR__)
  Wire.begin(); // OPTIONAL not needed if you use the 3rd begin below and are using default i2c pins for your board
#else
  Wire.begin(SDA,SCL); // OPTIONAL not needed if you use the 3rd begin below and are using default i2c pins for your board
#endif

  // initilize library with device i2c address.
  // consult datasheet for available addresses and use an i2c scan routine to verify
  rgbw.begin(0x60);

  // rgbw.begin(0x60, 50); // optionally set fade delay if using library to transistion between PWM values

  //rgbw.begin(0x60, 50, true); // set fade delay and also init the i2c bus using Arduino defaults for your board

  rgbw.setrgbw(128,128,128,128); // set all four outputs to 50%
  delay(500);
}

void loop() {

  rgbw.setpwm(0,255); // ramp channel 0 to 100%
  delay(500);

  rgbw.setpwm(0,0); // ramp channel 0 to 0%
  delay(500);

  rgbw.setpwm(1,255); // ramp channel 1 to 100%
  delay(500);

  rgbw.setpwm(1,0); // ramp channel 1 to 0%
  delay(500);

  rgbw.setpwm(2,255); // ramp channel 2 to 100%
  delay(500);

  rgbw.setpwm(2,0); // ramp channel 2 to 0%
  delay(500);
}
