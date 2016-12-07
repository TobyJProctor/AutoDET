#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
 
Adafruit_MotorShield AFMS = Adafruit_MotorShield();//is this line necessary?
Adafruit_MotorShield AFMS0(0x60);//binary0000, no jumpers

Adafruit_DCMotor *wmMotor30 = AFMS0.getMotor(4);
Adafruit_DCMotor *wmMotor200 = AFMS0.getMotor(1);

boolean wmMotor30Enabled = false;//Motor running or not
boolean wmMotor200Enabled = false;

unsigned long startMillis = 0;
unsigned long currentMillis = 0;
unsigned long elapsedPause = 0;
unsigned long elapsedMillis = 0;
unsigned long wmMotor200PrimeMillis = 5000;
unsigned long wmMotor30RunH2O72hMillis = 5000;


void setup() {
  // put your setup code here, to run once:
AFMS.begin();  // create with the default frequency 1.6KHz
AFMS0.begin(); // call shield 0

wmMotor30Enabled = false; // wmMotor Off
wmMotor200Enabled = false; // wmMotor Off
}

void loop() {
  // put your main code here, to run repeatedly:
    
    delay(5000);
    startMillis = millis();
    delay(10);
    currentMillis = millis();
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis){//H2O prime
      wmMotor200Enabled = true;// wmMotor200 on
      wmMotor200->run(FORWARD);//CLOCKWISE
      wmMotor200->setSpeed(100);
      wmMotor30Enabled = false;
      currentMillis = millis(); 
  }
    startMillis = millis();
    delay(10);
    currentMillis = millis();
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor30RunH2O72hMillis){//H2O prime
      wmMotor200Enabled = false;// wmMotor200 on
      wmMotor200->run(RELEASE);//CLOCKWISE
      wmMotor200->setSpeed(0);
      wmMotor30Enabled = true;
      wmMotor30->run(FORWARD);
      wmMotor30->setSpeed(100);
      currentMillis = millis(); 
  }
}
