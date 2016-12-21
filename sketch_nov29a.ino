#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
 
Adafruit_MotorShield AFMS = Adafruit_MotorShield();//is this line necessary?
Adafruit_MotorShield AFMS0(0x60);//binary0000, no jumpers
Adafruit_MotorShield AFMS1(0x61);//binary0001, bridge A0
Adafruit_MotorShield AFMS2(0x62);//binary0010, bridge A1
Adafruit_MotorShield AFMS3(0x63);//binary0011, bridge A0 & A1
 
Adafruit_DCMotor *wmMotor30 = AFMS0.getMotor(1);
Adafruit_DCMotor *wmMotor200 = AFMS0.getMotor(2);
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

const int switchPin1 = 2;
const int switchPin2 = 4;

int switchState1 = 0; //current state of switch
int previouswitchState1 = 0; //previous state of switch
int switchState2 = 0;
int previousSwitchState2 = 0;

unsigned long startMillis = 0;
unsigned long currentMillis = 0;
unsigned long startDelay = 5000;
unsigned long elapsedPause = 0;
unsigned long elapsedMillis = 0;
unsigned long wmMotor200PrimeMillis = 10000;
unsigned long wmMotor200DrainMillis = 10000;
unsigned long wmMotor200CleanMillis = 10000;
unsigned long wmMotor30RunH2O72hMillis = 10000;
unsigned long wmMotor30RunH2O12hMillis = 10000;
unsigned long wmMotor30RunH2O1hMillis = 10000;
unsigned long wmMotor30DNase3hMillis = 10000;
unsigned long wmMotor30SDC4hMillis = 10000;


void setup() {
  // put your setup code here, to run once:
AFMS.begin();  // create with the default frequency 1.6KHz
AFMS0.begin(); // call shield 0
AFMS1.begin(); // call shield 1
AFMS2.begin(); // call shield 2
AFMS3.begin(); // call shield 3
wmMotor30Enabled = false; // wmMotor Off
wmMotor200Enabled = false; // wmMotor Off
sMotorFilterEnabled = false; // NC valve
sMotorH2OEnabled = true;// NO valve
sMotorH2O->run(FORWARD);
sMotorH2O->setSpeed(255);
sMotorDNaseEnabled = false; // NC valve
sMotorSDCEnabled = false; // NC valve
sMotor3WEnabled = true; // wmMotor200 port open
sMotor3W->run(FORWARD);
sMotor3W->setSpeed(255);
sMotorWasteEnabled = false; // NO valve
 
int switchState1 = 0;
int switchState2 = 0;

pinMode(switchPin1, INPUT);
pinMode(switchPin2, INPUT);

digitalWrite(switchPin1, LOW);
digitalWrite(switchPin2, LOW);
}

unsigned long Pausa(){//pause function, all motors off and valves closed
  while (switchState2 == HIGH){
    switchState2 = digitalRead(switchPin2);
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
    if(switchState2 == LOW){
      elapsedMillis = millis();
      return elapsedMillis;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
 switchState1 = digitalRead(switchPin1);
 delay(10);
 switchState2 = digitalRead(switchPin2);
 delay(10);
  if((switchState1 == HIGH) && (switchState2 == LOW)){
    elapsedPause = 0;
    startMillis = millis();
    delay(10);
    currentMillis = millis();
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis){//H2O prime
      wmMotor200Enabled = true;// wmMotor200 on
      wmMotor200->run(BACKWARD);//CLOCKWISE
      wmMotor200->setSpeed(100);
      wmMotor30Enabled = false;
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = false;
      sMotorH2O->run(RELEASE);
      sMotorH2O->setSpeed(0);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = true;
      sMotorWaste->run(FORWARD);
      sMotorWaste->setSpeed(255);
        switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      } 
      currentMillis = millis();
    }       
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor30RunH2O72hMillis){//72h H2O run
      wmMotor30Enabled = true;// wmMotor30 on
      wmMotor30->run(FORWARD);//CLOCKWISE
      wmMotor30->setSpeed(100);
      wmMotor200Enabled = false;
      wmMotor200->run(RELEASE);
      wmMotor200->setSpeed(0);
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = false;
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotor3WEnabled = false;
      sMotor3W->run(RELEASE);
      sMotor3W->setSpeed(0);
      sMotorWasteEnabled = false;
      sMotorWaste->run(RELEASE);
      sMotorWaste->setSpeed(0);
        switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200DrainMillis){//H2O drain
      wmMotor30Enabled = false;// wmMotor30 off
      wmMotor30->run(RELEASE);
      wmMotor30->setSpeed(0);
      wmMotor200Enabled = true;
      wmMotor200->run(BACKWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = true;
      sMotorFilter->run(FORWARD);
      sMotorFilter->setSpeed(255);
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false; 
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200CleanMillis){//H2O clean
      wmMotor30Enabled = false;// wmMotor30 off
      wmMotor200Enabled = true;
      wmMotor200->run(FORWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = false;
      sMotorFilter->run(RELEASE);
      sMotorFilter->setSpeed(0);
      sMotorH2OEnabled = false;
      sMotorH2O->run(RELEASE);
      sMotorH2O->setSpeed(0);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false;
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
     }
     while((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis){//SDC prime
      wmMotor30Enabled = false;// wmMotor30 off
      wmMotor200Enabled = true;
      wmMotor200->run(BACKWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = true;
      sMotorSDC->run(FORWARD);
      sMotorSDC->setSpeed(255);
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = true;
      sMotorWaste->run(FORWARD);
      sMotorWaste->setSpeed(255);
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();    
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor30SDC4hMillis){//SDC run
      wmMotor30Enabled = true;
      wmMotor30->run(FORWARD);
      wmMotor30->setSpeed(100);
      wmMotor200Enabled = false;
      wmMotor200->run(RELEASE);
      wmMotor200->setSpeed(0);
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = true;
      sMotorSDC->run(FORWARD);
      sMotorSDC->setSpeed(255);
      sMotor3WEnabled = false;
      sMotor3W->run(RELEASE);
      sMotor3W->setSpeed(0);
      sMotorWasteEnabled = false;
      sMotorWaste->run(RELEASE);
      sMotorWaste->setSpeed(0);       
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200DrainMillis){//SDC drain
      wmMotor30Enabled = false;
      wmMotor30->run(RELEASE);
      wmMotor30->setSpeed(0);
      wmMotor200Enabled = true;
      wmMotor200->run(BACKWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = true;
      sMotorFilter->run(FORWARD);
      sMotorFilter->setSpeed(255);
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotorSDC->run(RELEASE);
      sMotorSDC->setSpeed(0);
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false;     
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();  
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200CleanMillis){//SDC clean
      wmMotor30Enabled = false;
      wmMotor200Enabled = true;
      wmMotor200->run(FORWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = false;
      sMotorFilter->run(RELEASE);
      sMotorFilter->setSpeed(0);
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = true;
      sMotorSDC->run(FORWARD);
      sMotorSDC->setSpeed(255);
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false;      
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis){//1h H2O prime
      wmMotor30Enabled = false;
      wmMotor200Enabled = true;
      wmMotor200->run(BACKWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = false;
      sMotorH2O->run(RELEASE);
      sMotorH2O->setSpeed(0);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotorSDC->run(RELEASE);
      sMotorSDC->setSpeed(0);
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = true;
      sMotorWaste->run(FORWARD);
      sMotorWaste->setSpeed(255);      
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor30RunH2O1hMillis){//1h H2O run
      wmMotor30Enabled = true;
      wmMotor30->run(FORWARD);
      wmMotor30->setSpeed(100);
      wmMotor200Enabled = false;
      wmMotor200->run(RELEASE);
      wmMotor200->setSpeed(0);
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = false;
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotor3WEnabled = false;
      sMotor3W->run(RELEASE);
      sMotor3W->setSpeed(0);
      sMotorWasteEnabled = false;
      sMotorWaste->run(RELEASE);
      sMotorWaste->setSpeed(0);      
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200DrainMillis){//1h H2O drain
      wmMotor30Enabled = false;//
      wmMotor30->run(RELEASE);
      wmMotor30->setSpeed(0);
      wmMotor200Enabled = true;
      wmMotor200->run(BACKWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = true;
      sMotorFilter->run(FORWARD);
      sMotorFilter->setSpeed(255);
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false; 
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
      while((currentMillis - (startMillis + elapsedPause)) < wmMotor200CleanMillis){//H2O clean
      wmMotor30Enabled = false;// wmMotor30 off
      wmMotor200Enabled = true;
      wmMotor200->run(FORWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = false;
      sMotorFilter->run(RELEASE);
      sMotorFilter->setSpeed(0);
      sMotorH2OEnabled = false;
      sMotorH2O->run(RELEASE);
      sMotorH2O->setSpeed(0);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false;       
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
      while((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis){//3h DNase prime
      wmMotor30Enabled = false;
      wmMotor200Enabled = true;
      wmMotor200->run(BACKWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = true;
      sMotorDNase->run(FORWARD);
      sMotorDNase->setSpeed(255);
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = true;
      sMotorWaste->run(FORWARD);
      sMotorWaste->setSpeed(255);      
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
     while((currentMillis - (startMillis + elapsedPause)) < wmMotor30DNase3hMillis){//3h DNase run
      wmMotor30Enabled = true;
      wmMotor30->run(FORWARD);
      wmMotor30->setSpeed(100);
      wmMotor200Enabled = false;
      wmMotor200->run(RELEASE);
      wmMotor200->setSpeed(0);
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = true;
      sMotorDNase->run(FORWARD);
      sMotorDNase->setSpeed(255);
      sMotorSDCEnabled = false;
      sMotor3WEnabled = false;
      sMotor3W->run(RELEASE);
      sMotor3W->setSpeed(0);
      sMotorWasteEnabled = false;
      sMotorWaste->run(RELEASE);
      sMotorWaste->setSpeed(0);      
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
     while((currentMillis - (startMillis + elapsedPause)) < wmMotor200DrainMillis){//3h DNase drain
      wmMotor30Enabled = false;
      wmMotor30->run(RELEASE);
      wmMotor30->setSpeed(0);
      wmMotor200Enabled = true;
      wmMotor200->run(BACKWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = true;
      sMotorFilter->run(FORWARD);
      sMotorFilter->setSpeed(255);
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = false;
      sMotorDNase->run(RELEASE);
      sMotorDNase->setSpeed(0);
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false;   
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
     }
     while((currentMillis - (startMillis + elapsedPause)) < wmMotor200CleanMillis){//3h DNase clean
      wmMotor30Enabled = false;
      wmMotor200Enabled = true;
      wmMotor200->run(FORWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = false;
      sMotorFilter->run(RELEASE);
      sMotorFilter->setSpeed(0);
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = true;
      sMotorDNase->run(FORWARD);
      sMotorDNase->setSpeed(255);
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false;   
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
     }
     while((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis){//1h H2O prime
      wmMotor30Enabled = false;
      wmMotor200Enabled = true;
      wmMotor200->run(BACKWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = false;
      sMotorH2O->run(RELEASE);
      sMotorH2O->setSpeed(0);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotorSDC->run(RELEASE);
      sMotorSDC->setSpeed(0);
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = true;
      sMotorWaste->run(FORWARD);
      sMotorWaste->setSpeed(255);      
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor30RunH2O12hMillis){//12h H2O run
      wmMotor30Enabled = true;
      wmMotor30->run(FORWARD);
      wmMotor30->setSpeed(100);
      wmMotor200Enabled = false;
      wmMotor200->run(RELEASE);
      wmMotor200->setSpeed(0);
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = false;
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotor3WEnabled = false;
      sMotor3W->run(RELEASE);
      sMotor3W->setSpeed(0);
      sMotorWasteEnabled = false;
      sMotorWaste->run(RELEASE);
      sMotorWaste->setSpeed(0);      
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200DrainMillis){//H2O drain
      wmMotor30Enabled = false;//
      wmMotor30->run(RELEASE);
      wmMotor30->setSpeed(0);
      wmMotor200Enabled = true;
      wmMotor200->run(BACKWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = true;
      sMotorFilter->run(FORWARD);
      sMotorFilter->setSpeed(255);
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false; 
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
      while((currentMillis - (startMillis + elapsedPause)) < wmMotor200CleanMillis){//H2O clean
      wmMotor30Enabled = false;// wmMotor30 off
      wmMotor200Enabled = true;
      wmMotor200->run(FORWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = false;
      sMotorFilter->run(RELEASE);
      sMotorFilter->setSpeed(0);
      sMotorH2OEnabled = false;
      sMotorH2O->run(RELEASE);
      sMotorH2O->setSpeed(0);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false;       
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis){//SDC prime
      wmMotor30Enabled = false;// wmMotor30 off
      wmMotor200Enabled = true;
      wmMotor200->run(BACKWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = true;
      sMotorSDC->run(FORWARD);
      sMotorSDC->setSpeed(255);
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = true;
      sMotorWaste->run(FORWARD);
      sMotorWaste->setSpeed(255);       
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();    
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor30SDC4hMillis){//SDC run
      wmMotor30Enabled = true;
      wmMotor30->run(FORWARD);
      wmMotor30->setSpeed(100);
      wmMotor200Enabled = false;
      wmMotor200->run(RELEASE);
      wmMotor200->setSpeed(0);
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = true;
      sMotorSDC->run(FORWARD);
      sMotorSDC->setSpeed(255);
      sMotor3WEnabled = false;
      sMotor3W->run(RELEASE);
      sMotor3W->setSpeed(0);
      sMotorWasteEnabled = false;
      sMotorWaste->run(RELEASE);
      sMotorWaste->setSpeed(0);       
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200DrainMillis){//SDC drain
      wmMotor30Enabled = false;
      wmMotor30->run(RELEASE);
      wmMotor30->setSpeed(0);
      wmMotor200Enabled = true;
      wmMotor200->run(BACKWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = true;
      sMotorFilter->run(FORWARD);
      sMotorFilter->setSpeed(255);
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotorSDC->run(RELEASE);
      sMotorSDC->setSpeed(0);
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false;     
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();  
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200CleanMillis){//SDC clean
      wmMotor30Enabled = false;
      wmMotor200Enabled = true;
      wmMotor200->run(FORWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = false;
      sMotorFilter->run(RELEASE);
      sMotorFilter->setSpeed(0);
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = true;
      sMotorSDC->run(FORWARD);
      sMotorSDC->setSpeed(255);
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false;      
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis){//1h H2O prime
      wmMotor30Enabled = false;
      wmMotor200Enabled = true;
      wmMotor200->run(BACKWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = false;
      sMotorH2O->run(RELEASE);
      sMotorH2O->setSpeed(0);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotorSDC->run(RELEASE);
      sMotorSDC->setSpeed(0);
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = true;
      sMotorWaste->run(FORWARD);
      sMotorWaste->setSpeed(255);      
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor30RunH2O1hMillis){//1h H2O run
      wmMotor30Enabled = true;
      wmMotor30->run(FORWARD);
      wmMotor30->setSpeed(100);
      wmMotor200Enabled = false;
      wmMotor200->run(RELEASE);
      wmMotor200->setSpeed(0);
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = false;
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotor3WEnabled = false;
      sMotor3W->run(RELEASE);
      sMotor3W->setSpeed(0);
      sMotorWasteEnabled = false;
      sMotorWaste->run(RELEASE);
      sMotorWaste->setSpeed(0);      
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200DrainMillis){//1h H2O drain
      wmMotor30Enabled = false;//
      wmMotor30->run(RELEASE);
      wmMotor30->setSpeed(0);
      wmMotor200Enabled = true;
      wmMotor200->run(BACKWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = true;
      sMotorFilter->run(FORWARD);
      sMotorFilter->setSpeed(255);
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false; 
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
      while((currentMillis - (startMillis + elapsedPause)) < wmMotor200CleanMillis){//H2O clean
      wmMotor30Enabled = false;// wmMotor30 off
      wmMotor200Enabled = true;
      wmMotor200->run(FORWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = false;
      sMotorFilter->run(RELEASE);
      sMotorFilter->setSpeed(0);
      sMotorH2OEnabled = false;
      sMotorH2O->run(RELEASE);
      sMotorH2O->setSpeed(0);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false;       
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
      while((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis){//3h DNase prime
      wmMotor30Enabled = false;
      wmMotor200Enabled = true;
      wmMotor200->run(BACKWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = true;
      sMotorDNase->run(FORWARD);
      sMotorDNase->setSpeed(255);
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = true;
      sMotorWaste->run(FORWARD);
      sMotorWaste->setSpeed(255);      
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
     while((currentMillis - (startMillis + elapsedPause)) < wmMotor30DNase3hMillis){//3h DNase run
      wmMotor30Enabled = true;
      wmMotor30->run(FORWARD);
      wmMotor30->setSpeed(100);
      wmMotor200Enabled = false;
      wmMotor200->run(RELEASE);
      wmMotor200->setSpeed(0);
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = true;
      sMotorDNase->run(FORWARD);
      sMotorDNase->setSpeed(255);
      sMotorSDCEnabled = false;
      sMotor3WEnabled = false;
      sMotor3W->run(RELEASE);
      sMotor3W->setSpeed(0);
      sMotorWasteEnabled = false;
      sMotorWaste->run(RELEASE);
      sMotorWaste->setSpeed(0);      
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
     while((currentMillis - (startMillis + elapsedPause)) < wmMotor200DrainMillis){//3h DNase drain
      wmMotor30Enabled = false;
      wmMotor30->run(RELEASE);
      wmMotor30->setSpeed(0);
      wmMotor200Enabled = true;
      wmMotor200->run(BACKWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = true;
      sMotorFilter->run(FORWARD);
      sMotorFilter->setSpeed(255);
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = false;
      sMotorDNase->run(RELEASE);
      sMotorDNase->setSpeed(0);
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false;   
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
     }
     while((currentMillis - (startMillis + elapsedPause)) < wmMotor200CleanMillis){//3h DNase clean
      wmMotor30Enabled = false;
      wmMotor200Enabled = true;
      wmMotor200->run(FORWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = false;
      sMotorFilter->run(RELEASE);
      sMotorFilter->setSpeed(0);
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = true;
      sMotorDNase->run(FORWARD);
      sMotorDNase->setSpeed(255);
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false;   
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
     }
     while((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis){//H2O prime
      wmMotor200Enabled = true;// wmMotor200 on
      wmMotor200->run(BACKWARD);
      wmMotor200->setSpeed(100);
      wmMotor30Enabled = false;
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = false;
      sMotorH2O->run(RELEASE);
      sMotorH2O->setSpeed(0);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = true;
      sMotorWaste->run(FORWARD);
      sMotorWaste->setSpeed(255);
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor30RunH2O72hMillis){//72h H2O run
      wmMotor30Enabled = true;// wmMotor30 on
      wmMotor30->run(FORWARD);
      wmMotor30->setSpeed(100);
      wmMotor200Enabled = false;
      wmMotor200->run(RELEASE);
      wmMotor200->setSpeed(0);
      sMotorFilterEnabled = false;
      sMotorH2OEnabled = false;
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotor3WEnabled = false;
      sMotor3W->run(RELEASE);
      sMotor3W->setSpeed(0);
      sMotorWasteEnabled = false;
      sMotorWaste->run(RELEASE);
      sMotorWaste->setSpeed(0); 
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200DrainMillis){//H2O drain
      wmMotor30Enabled = false;// wmMotor30 off
      wmMotor30->run(RELEASE);
      wmMotor30->setSpeed(0);
      wmMotor200Enabled = true;
      wmMotor200->run(BACKWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = true;
      sMotorFilter->run(FORWARD);
      sMotorFilter->setSpeed(255);
      sMotorH2OEnabled = true;
      sMotorH2O->run(FORWARD);
      sMotorH2O->setSpeed(255);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false; 
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
    }
    while((currentMillis - (startMillis + elapsedPause)) < wmMotor200CleanMillis){//H2O clean
      wmMotor30Enabled = false;// wmMotor30 off
      wmMotor200Enabled = true;
      wmMotor200->run(FORWARD);
      wmMotor200->setSpeed(100);
      sMotorFilterEnabled = false;
      sMotorFilter->run(RELEASE);
      sMotorFilter->setSpeed(0);
      sMotorH2OEnabled = false;
      sMotorH2O->run(RELEASE);
      sMotorH2O->setSpeed(0);
      sMotorDNaseEnabled = false;
      sMotorSDCEnabled = false;
      sMotor3WEnabled = true;
      sMotor3W->run(FORWARD);
      sMotor3W->setSpeed(255);
      sMotorWasteEnabled = false;       
              switchState1 = digitalRead(switchPin1);
        if(switchState1 == LOW){
         break;
        }
        switchState2 = digitalRead(switchPin2);
        if(switchState2 == HIGH){
         elapsedMillis = Pausa();
      }
      currentMillis = millis();
     }
   }
   else {
    wmMotor30Enabled = false; // wmMotor Off
  wmMotor200Enabled = false; // wmMotor Off
  sMotorFilterEnabled = false; // NC valve
  sMotorH2OEnabled = true;// NO valve
  sMotorH2O->run(FORWARD);
  sMotorH2O->setSpeed(255);
  sMotorDNaseEnabled = false; // NC valve
  sMotorSDCEnabled = false; // NC valve
  sMotor3WEnabled = true; // wmMotor200 port open
  sMotor3W->run(FORWARD);
  sMotor3W->setSpeed(255);
  sMotorWasteEnabled = false; // NO valve
   }
}
