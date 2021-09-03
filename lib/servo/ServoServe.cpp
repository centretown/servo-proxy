// Copyright (c) 2021 Dave Marsh. See LICENSE.

/*************************************************** 
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 8 servos, one after the other on the
  first 8 pins of the PCA9685

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815
  
  These drivers use I2C to communicate, 2 pins are required to  
  interface.

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "ServoServe.h"

int ServoServe::setup()
{
    // Serial.println("pwm.begin();");
    pwm.begin();
    /*
   * In theory the internal oscillator (clock) is 25MHz but it really isn't
   * that precise. You can 'calibrate' this by tweaking this number until
   * you get the PWM update frequency you're expecting!
   * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
   * is used for calculating things like writeMicroseconds()
   * Analog servos run at ~50 Hz updates, It is importaint to use an
   * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
   * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
   *    the I2C PCA9685 chip you are setting the value for.
   * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
   *    expected value (50Hz for most ESCs)
   * Setting the value here is specific to each individual I2C PCA9685 chip and
   * affects the calculations for the PWM update frequency. 
   * Failure to correctly set the int.osc value will cause unexpected PWM results
   */
    // Serial.println("pwm.setOscillatorFrequency(27000000);");
    pwm.setOscillatorFrequency(25000000);
    // Serial.println("pwm.setPWMFreq(SERVO_FREQ);");
    pwm.setPWMFreq(SERVO_FREQ); // Analog servos run at ~50 Hz updates
    delay(10);
    return 0;
}

int ServoServe::start()
{
    loopServo();
    // wave(10);

    return 0;
}

// You can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!
void ServoServe::setServoPulse(uint8_t n, double pulse)
{
    double pulselength;

    pulselength = 1000000;     // 1,000,000 us per second
    pulselength /= SERVO_FREQ; // Analog servos run at ~60 Hz updates
    // Serial.print(pulselength);
    // Serial.println(" us per period");
    pulselength /= 4096; // 12 bits of resolution
    // Serial.print(pulselength);
    // Serial.println(" us per bit");
    pulse *= 1000000; // convert input seconds to us
    pulse /= pulselength;
    // Serial.println(pulse);
    pwm.setPWM(n, 0, pulse);
}

void ServoServe::loopServo(int count)
{
    pulseTest(count);
    microTest(count);
    return;
}

void ServoServe::pulseTest(int count)
{
    // Drive each servo one at a time using setPWM()
    // Serial.println(servoNum);
    for (int i = 0; i < count; i++)
    {
        for (servoNum = 0; servoNum < servoCount; servoNum++)
        {
            for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++)
            {
                pwm.setPWM(servoNum, 0, pulselen);
            }
            delay(500);
            for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--)
            {
                pwm.setPWM(servoNum, 0, pulselen);
            }
            delay(500);
        }
    }
}

void ServoServe::microTest(int count)
{
    // Drive each servo one at a time using writeMicroseconds(), it's not precise due to calculation rounding!
    // The writeMicroseconds() function is used to mimic the Arduino Servo library writeMicroseconds() behavior.
    for (int i = 0; i < count; i++)
    {
        for (servoNum = 0; servoNum < servoCount; servoNum++)
        {
            for (uint16_t microsec = USMIN; microsec < USMAX; microsec++)
            {
                pwm.writeMicroseconds(servoNum, microsec);
            }
            delay(500);
            for (uint16_t microsec = USMAX; microsec > USMIN; microsec--)
            {
                pwm.writeMicroseconds(servoNum, microsec);
            }
            delay(500);
        }
    }
}

void ServoServe::wave(int count)
{
    for (int i = 0; i < count; i++)
    {
        // Drive each PWM in a 'wave'
        for (uint16_t i = 0; i < 4096; i += 8)
        {
            for (uint8_t pwmnum = 0; pwmnum < servoCount; pwmnum++)
            {
                pwm.setPWM(pwmnum, 0, (i + (4096 / 16) * pwmnum) % 4096);
            }
        }
    }
}
