/*This code runs all motors, consecutively, for 5 s each.
There is no switch required for operation.
At the end of loop the pumps should be turned off permanently.*/ 

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
 
Adafruit_MotorShield AFMS = Adafruit_MotorShield();//is this line necessary?
Adafruit_MotorShield AFMS0(0x60);//binary0000, no jumpers
Adafruit_MotorShield AFMS1(0x61);//binary0001, bridge A0
Adafruit_MotorShield AFMS2(0x62);//binary0010, bridge A1
Adafruit_MotorShield AFMS3(0x63);//binary0011, bridge A0 & A1

Adafruit_DCMotor *wmMotor30 = AFMS0.getMotor(4);
Adafruit_DCMotor *wmMotor200 = AFMS0.getMotor(1);
Adafruit_DCMotor *sMotorFilter = AFMS1.getMotor(1);
Adafruit_DCMotor *sMotorH2O = AFMS1.getMotor(2);
Adafruit_DCMotor *sMotorDNase = AFMS2.getMotor(1);
Adafruit_DCMotor *sMotorSDC = AFMS2.getMotor(2);
Adafruit_DCMotor *sMotor3W = AFMS3.getMotor(1);
Adafruit_DCMotor *sMotorWaste = AFMS3.getMotor(2);

boolean wmMotor30Enabled = false;//Motor running or not
boolean wmMotor200Enabled = false;
boolean sMotorFilterEnabled = false;
boolean sMotorH2OEnabled = false;
boolean sMotorDNaseEnabled = false;
boolean sMotorSDCEnabled = false;
boolean sMotor3WEnabled = false;
boolean sMotorWasteEnabled = false;

unsigned long startMillis = 0;
unsigned long currentMillis = 0;
unsigned long startDelay = 5000;
unsigned long elapsedPause = 0;
unsigned long elapsedMillis = 0;
unsigned long wmMotor200PrimeMillis = 10000;
unsigned long wmMotor30RunH2O72hMillis = 5000;
unsigned long wmMotor30RunH2O12hMillis = 5000;
unsigned long wmMotor30RunH2O1hMillis = 5000;
unsigned long wmMotor30DNase3hMillis = 5000;
unsigned long wmMotor30SDC4hMillis = 5000;

const int switchPin2 = 4;
int switchState2 = HIGH;

    unsigned long Pausa(){//pause function, all motors off and valves closed
  //while (switchState2 == LOW){
    //switchState2 = digitalRead(switchPin2);
    wmMotor30Enabled = false;
    wmMotor200Enabled = false;
    sMotorFilterEnabled = false;
    sMotorH2OEnabled = true;
    sMotorH2O->run(FORWARD);
    sMotorH2O->setSpeed(255);
    sMotorDNaseEnabled = false;
    sMotorSDCEnabled = false;
    sMotor3WEnabled = false;
    sMotorWasteEnabled = true;
    sMotorWaste->run(FORWARD);
    sMotorWaste->setSpeed(255);

    delay(120000);
    //if(switchState2 == HIGH){
      //elapsedMillis = millis();
      //return elapsedMillis;
    //}
  //}
}

void setup() {
  // put your setup code here, to run once:
AFMS.begin();  // create with the default frequency 1.6KHz
AFMS0.begin(); // call shield 0
AFMS1.begin();
AFMS2.begin();
AFMS3.begin();

wmMotor30Enabled = false; // wmMotor Off
wmMotor200Enabled = false; // wmMotor Off
sMotorFilterEnabled = false; // NC valve
sMotorH2OEnabled = false;// NO valve
sMotorDNaseEnabled = false; // NC valve
sMotorSDCEnabled = false; // NC valve
sMotor3WEnabled = false; // wmMotor200 port closed
sMotorWasteEnabled = false; // NO valve

pinMode(switchPin2, INPUT_PULLUP);
int switchState2 = HIGH;
//digitalWrite(switchPin2, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
    switchState2 = digitalRead(switchPin2);
    delay(10);
    if((switchState2 == HIGH)){
    elapsedPause = 0;
    delay(5000);
    startMillis = millis();
    delay(10);
    currentMillis = millis();
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis){//wmMotor200
      wmMotor200Enabled = true;// wmMotor200 on
      wmMotor200->run(FORWARD);//CLOCKWISE
      wmMotor200->setSpeed(100);
      wmMotor30Enabled = false;
      sMotorFilterEnabled = false; // NC valve
      sMotorH2OEnabled = false;// NO valve
      sMotorDNaseEnabled = false; // NC valve
      sMotorSDCEnabled = false; // NC valve
      sMotor3WEnabled = false; // wmMotor200 port closed
      sMotorWasteEnabled = false; // NO valve

        switchState2 = digitalRead(switchPin2);
        delay(200);
        if(switchState2 == LOW){
          delay(300);
         elapsedMillis = Pausa();
      } 
      currentMillis = millis(); 
  }
    startMillis = millis();
    delay(10);
    currentMillis = millis();
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor30RunH2O72hMillis){//wmMotor200
      wmMotor200Enabled = true;// wmMotor200 on
      wmMotor200->run(FORWARD);//CLOCKWISE
      wmMotor200->setSpeed(100);
      wmMotor30Enabled = false;
      sMotorFilterEnabled = false; // NC valve
      sMotorH2OEnabled = true;// NO valve
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = false; // NC valve
      sMotorSDCEnabled = false; // NC valve
      sMotor3WEnabled = false; // wmMotor200 port closed
      sMotorWasteEnabled = false; // NO valve

        switchState2 = digitalRead(switchPin2);
        delay(200);
        if(switchState2 == LOW){
          delay(300);
         elapsedMillis = Pausa();
      } 
      currentMillis = millis(); 
  }
  startMillis = millis();
    delay(10);
    currentMillis = millis();
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis){//wmMotor200
      wmMotor200Enabled = true;// wmMotor200 on
      wmMotor200->run(FORWARD);//CLOCKWISE
      wmMotor200->setSpeed(100);
      wmMotor30Enabled = false;
      sMotorFilterEnabled = false; // NC valve
      sMotorH2OEnabled = false;// NO valve
      sMotorH2O->run(RELEASE);
      sMotorH2O->setSpeed(0);
      sMotorDNaseEnabled = false; // NC valve
      sMotorSDCEnabled = false; // NC valve
      sMotor3WEnabled = false; // wmMotor200 port closed
      sMotorWasteEnabled = false; // NO valve

        switchState2 = digitalRead(switchPin2);
        delay(200);
        if(switchState2 == LOW){
          delay(300);
         elapsedMillis = Pausa();
      } 
      currentMillis = millis(); 
  }
      currentMillis = millis();
    while(currentMillis > 0){
      wmMotor200Enabled = false;// wmMotor200 on
      wmMotor30Enabled = false;
      sMotorFilterEnabled = false; // NC valve
      sMotorH2OEnabled = false;// NO valve
      sMotorDNaseEnabled = false; // NC valve
      sMotorSDCEnabled = false; // NC valve
      sMotor3WEnabled = false; // wmMotor200 port open
      sMotor3W->run(RELEASE);
      sMotor3W->setSpeed(0);
      sMotorWasteEnabled = false; // NO valve
      sMotorWaste->run(RELEASE);
      sMotorWaste->setSpeed(0);
      currentMillis = millis();
      switchState2 = digitalRead(switchPin2);
        if(switchState2 == LOW){
         elapsedMillis = Pausa();
  }
  }
    }}
