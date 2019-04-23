## pca9633

Library to control the NXP / Philips / Texas Instruments PCA9633 four channel 8 bit LED control chip. This chip has four LED driver channels each with its own PWM brightness control, as well as a separate global brightness control. Can be used to control RGB+W LED strips if you use external drivers (transistor, etc.)

---
### Quick start:

```c
// include library
#include <pca9633.h>
#include <Wire.h> // OPTIONAL see below under setup()

// create instance
PCA9633 rgbw; 

void setup() {
	Wire.begin(SDA,SCL); // OPTIONAL not needed if you use the 3rd begin below and are using default i2c pins for your board
	
	// initilize library with device i2c address.
	// consult datasheet for available addresses and use an i2c scan routine to verify
	rgbw.begin(0x60);

	// rgbw.begin(0x60, 50); // optionally set fade delay if using library to transistion between PWM values
	
	// rgbw.begin(0x60, 50, true); // set fade delay and also init the i2c bus using Arduino defaults for your board
}

void loop() {
	rgbw.setrgbw(128,128,128,128); // set all four outputs to 50%
	delay(500);

	rgbw.setrgbw(0,0,0,0); // set all four outputs to 0%
	delay(500);

	rgbw.setpwm(0,255); // ramp channel 0 to 100%
	delay(500);

	rgbw.setpwm(0,0); // ramp channel 0 to 0%
	delay(500);
}
```
---

### Some functions:

```c
// blocking call, fade single channel from current PWM setting to new PWM setting
rgbw.setpwm(uint8_t pwmaddr, uint8_t pwmval); 

// non blocking call, set all four channels in one command, no fade
rgbw.setrgbw(uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3);

// update fade step delay, range 0 to 100, default is 10
rgbw.setFade(uint8_t dly);

// retreive current fade delay;
uint8_t fadeDelay = rgbw.getFade(); 

// set global dimmer pwm. will dim all outputs, consult datasheet
rgbw.setgrouppwm(uint8_t pwm);

// init library with slave device address
rgbw.begin(uint8_t devAddr);

// init library with slave device and fade delay
rgbw.begin(uint8_t devAddr, uint8_t fade_delay);

```

### My prototype board:
ESP8266 based 4-channel dimmer for large LED strips. Beefy mosfets on each channel with proper 12v drivers.

![Image of PCA9633 prototype board](https://github.com/gordonthree/pca9633/blob/master/pca9633-proto.png?raw=true)
