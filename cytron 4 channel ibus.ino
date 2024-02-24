#include <IBusBM.h>
#include "CytronMotorDriver.h"

// Create iBus Object
IBusBM ibus;

// Channel Values
int rcCH1 = 0; // Left - Right
int rcCH2 = 0; // Forward - Reverse
int rcCH3 = 0; // Acceleration
int rcCH4 = 0;
int rcCH5 = 0; // Spin Control
bool rcCH6 = 0; // Mode Control

// Create Cytron Object
CytronMD motor1(PWM_DIR, 2, 3);
CytronMD motor2(PWM_DIR, 4, 5);
CytronMD motor3(PWM_DIR, 6, 7);
CytronMD motor4(PWM_DIR, 8, 9);

//Read Channel and return int value
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue) {
  uint16_t ch = ibus.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Read the channel and return a boolean value
bool readSwitch(byte channelInput, bool defaultValue) {
  int intDefaultValue = (defaultValue) ? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

void setup() {
  Serial.begin(115200);
  // Attach iBus object to serial port
  ibus.begin(Serial);
}

void loop() {
  //Read all channels
  rcCH1 = readChannel(0, -100, 100, 0);
  rcCH2 = readChannel(1, -100, 100, 0);
  rcCH3 = readChannel(2, 0, 155, 0);
  rcCH4 = readChannel(3, -100, 100, 0);
  rcCH5 = readSwitch(5, false);
  rcCH6 = readSwitch(6, false);

  Serial.print("Ch1 = ");
  Serial.print(rcCH1);
  Serial.print(" Ch2 = ");
  Serial.print(rcCH2);
  Serial.print(" Ch3 = ");
  Serial.print(rcCH3);
  Serial.print(" Ch4 = ");
  Serial.print(rcCH4);
  Serial.print(" Ch5 = ");
  Serial.print(rcCH5);
  Serial.print(" Ch6 = ");
  Serial.println(rcCH6);
  
  if (rcCH1 != 0) {
    int rcCH1_1 = map(rcCH1, -100, 100, -255, 255);
    if (rcCH1_1 < 0) {  // left
      motor1.setSpeed(0);
      motor2.setSpeed(rcCH1_1);
      motor4.setSpeed(0);
      motor3.setSpeed(rcCH1_1);
      Serial.println("left");
      Serial.println(rcCH1_1);
    } else {
      //right
      motor1.setSpeed(rcCH1_1);
      motor2.setSpeed(0);
      motor3.setSpeed(0);
      motor4.setSpeed(rcCH1_1);
      Serial.println("right");
      Serial.println(rcCH1_1);
    }
  } else if (rcCH2 != 0) {
    int rcCH2_1 = map(rcCH2, -100, 100, -255, 255);
    
    if (rcCH2 > 8) {
      motor1.setSpeed(rcCH2_1);
      motor2.setSpeed(rcCH2_1);
      motor3.setSpeed(rcCH2_1);
      motor4.setSpeed(rcCH2_1);
      Serial.println("forward");
      Serial.println(rcCH2_1);
    } else if (rcCH2 < -8) {
      motor1.setSpeed(rcCH2_1);
      motor2.setSpeed(rcCH2_1);
      motor3.setSpeed(rcCH2_1);
      motor4.setSpeed(rcCH2_1);
      Serial.println("backward");
      Serial.println(rcCH2_1);
    } 
  }
    if (rcCH5 == 1) {
      motor1.setSpeed(0);
      motor2.setSpeed(0);
      motor3.setSpeed(0);
      motor4.setSpeed(0);
      
    }
  
}
