/*This code dictates a full run of the DET DC protocol. Waste removal is achieved through DRAINING.
   Each motor actuation period has been limited to 20s.
   At the end of the loop, the operation is halted but the chamber is not drained. */

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();//is this line necessary?
Adafruit_MotorShield AFMS0(0x60);//binary0000, no jumpers
Adafruit_MotorShield AFMS1(0x61);//binary0001, bridge A0
Adafruit_MotorShield AFMS2(0x62);//binary0010, bridge A1
Adafruit_MotorShield AFMS3(0x63);//binary0011, bridge A0 & A1

Adafruit_DCMotor *wmMotor30 = AFMS0.getMotor(2);
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
unsigned long wmMotor200PrimeMillis = 20000;
unsigned long wmMotor200DrainMillis = 20000;
unsigned long wmMotor200CleanMillis = 2000;
unsigned long wmMotor30RunH2O72hMillis = 20000;
unsigned long wmMotor30RunH2O12hMillis = 20000;
unsigned long wmMotor30RunH2O1hMillis = 20000;
unsigned long wmMotor30DNase3hMillis = 20000;
unsigned long wmMotor30SDC4hMillis = 20000;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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
}
void loop() {
  // put your main code here, to run repeatedly:
  delay(startDelay);
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis) { //H2O prime
    wmMotor200Enabled = true;// wmMotor200 on
    wmMotor200->run(FORWARD);//CLOCKWISE
    wmMotor200->setSpeed(150);
    wmMotor30Enabled = false;
    sMotorFilterEnabled = true;
    sMotorFilter->run(FORWARD);
    sMotorFilter->setSpeed(255);
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

    currentMillis = millis();
    Serial.println("H2O prime");
  }

  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor30RunH2O72hMillis) { //72h H2O run
    wmMotor30Enabled = true;// wmMotor30 on
    wmMotor30->run(FORWARD);//CLOCKWISE
    wmMotor30->setSpeed(202);
    wmMotor200Enabled = false;
    wmMotor200->run(RELEASE);
    wmMotor200->setSpeed(0);
    sMotorFilterEnabled = false;
    sMotorFilter->run(RELEASE);
    sMotorFilter->setSpeed(0);
    sMotorH2OEnabled = false;
    sMotorDNaseEnabled = false;
    sMotorSDCEnabled = false;
    sMotor3WEnabled = false;
    sMotor3W->run(RELEASE);
    sMotor3W->setSpeed(0);
    sMotorWasteEnabled = false;
    sMotorWaste->run(RELEASE);
    sMotorWaste->setSpeed(0);

    currentMillis = millis();
    Serial.print("72h H2O Run");
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200DrainMillis) { //H2O drain
    wmMotor30Enabled = false;// wmMotor30 off
    wmMotor30->run(RELEASE);
    wmMotor30->setSpeed(0);
    wmMotor200Enabled = false;
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

    currentMillis = millis();
    Serial.print("H2O Drain");
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200CleanMillis) { //H2O clean
    wmMotor30Enabled = false;// wmMotor30 off
    wmMotor200Enabled = true;
    wmMotor200->run(FORWARD);
    wmMotor200->setSpeed(150);
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis) { //SDC prime
    wmMotor30Enabled = false;// wmMotor30 off
    wmMotor200Enabled = true;
    wmMotor200->run(FORWARD);
    wmMotor200->setSpeed(150);
    sMotorFilterEnabled = true;
    sMotorFilter->run(FORWARD);
    sMotorFilter->setSpeed(255);
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor30SDC4hMillis) { //SDC run
    wmMotor30Enabled = true;
    wmMotor30->run(FORWARD);
    wmMotor30->setSpeed(202);
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200DrainMillis) { //SDC drain
    wmMotor30Enabled = false;
    wmMotor30->run(RELEASE);
    wmMotor30->setSpeed(0);
    wmMotor200Enabled = false;
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200CleanMillis) { //SDC clean
    wmMotor30Enabled = false;
    wmMotor200Enabled = true;
    wmMotor200->run(FORWARD);
    wmMotor200->setSpeed(150);
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis) { //1h H2O prime
    wmMotor30Enabled = false;
    wmMotor200Enabled = true;
    wmMotor200->run(FORWARD);
    wmMotor200->setSpeed(150);
    sMotorFilterEnabled = true;
    sMotorFilter->run(FORWARD);
    sMotorFilter->setSpeed(255);
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor30RunH2O1hMillis) { //1h H2O run
    wmMotor30Enabled = true;
    wmMotor30->run(FORWARD);
    wmMotor30->setSpeed(202);
    wmMotor200Enabled = false;
    wmMotor200->run(RELEASE);
    wmMotor200->setSpeed(0);
    sMotorFilterEnabled = false;
    sMotorFilter->run(RELEASE);
    sMotorFilter->setSpeed(0);
    sMotorH2OEnabled = false;
    sMotorDNaseEnabled = false;
    sMotorSDCEnabled = false;
    sMotor3WEnabled = false;
    sMotor3W->run(RELEASE);
    sMotor3W->setSpeed(0);
    sMotorWasteEnabled = false;
    sMotorWaste->run(RELEASE);
    sMotorWaste->setSpeed(0);

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200DrainMillis) { //1h H2O drain
    wmMotor30Enabled = false;//
    wmMotor30->run(RELEASE);
    wmMotor30->setSpeed(0);
    wmMotor200Enabled = false;
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200CleanMillis) { //H2O clean
    wmMotor30Enabled = false;// wmMotor30 off
    wmMotor200Enabled = true;
    wmMotor200->run(FORWARD);
    wmMotor200->setSpeed(150);
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis) { //3h DNase prime
    wmMotor30Enabled = false;
    wmMotor200Enabled = true;
    wmMotor200->run(FORWARD);
    wmMotor200->setSpeed(150);
    sMotorFilterEnabled = true;
    sMotorFilter->run(FORWARD);
    sMotorFilter->setSpeed(255);
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor30DNase3hMillis) { //3h DNase run
    wmMotor30Enabled = true;
    wmMotor30->run(FORWARD);
    wmMotor30->setSpeed(202);
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200DrainMillis) { //3h DNase drain
    wmMotor30Enabled = false;
    wmMotor30->run(RELEASE);
    wmMotor30->setSpeed(0);
    wmMotor200Enabled = false;
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200CleanMillis) { //3h DNase clean
    wmMotor30Enabled = false;
    wmMotor200Enabled = true;
    wmMotor200->run(FORWARD);
    wmMotor200->setSpeed(150);
    sMotorFilterEnabled = false;
    sMotorFilter->run(RELEASE);
    sMotorFilter->setSpeed(0);
    sMotorH2OEnabled = false;
    sMotorH2O->run(RELEASE);
    sMotorH2O->setSpeed(0);
    sMotorDNaseEnabled = false;
    sMotorDNase->run(RELEASE);
    sMotorDNase->setSpeed(0);
    sMotorSDCEnabled = false;
    sMotor3WEnabled = true;
    sMotor3W->run(FORWARD);
    sMotor3W->setSpeed(255);
    sMotorWasteEnabled = false;

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis) { //1h H2O prime
    wmMotor30Enabled = false;
    wmMotor200Enabled = true;
    wmMotor200->run(FORWARD);
    wmMotor200->setSpeed(150);
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor30RunH2O12hMillis) { //12h H2O run
    wmMotor30Enabled = true;
    wmMotor30->run(FORWARD);
    wmMotor30->setSpeed(202);
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200DrainMillis) { //H2O drain
    wmMotor30Enabled = false;//
    wmMotor30->run(RELEASE);
    wmMotor30->setSpeed(0);
    wmMotor200Enabled = false;
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200CleanMillis) { //H2O clean
    wmMotor30Enabled = false;// wmMotor30 off
    wmMotor200Enabled = true;
    wmMotor200->run(FORWARD);
    wmMotor200->setSpeed(150);
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis) { //SDC prime
    wmMotor30Enabled = false;// wmMotor30 off
    wmMotor200Enabled = true;
    wmMotor200->run(FORWARD);
    wmMotor200->setSpeed(150);
    sMotorFilterEnabled = true;
    sMotorFilter->run(FORWARD);
    sMotorFilter->setSpeed(255);
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor30SDC4hMillis) { //SDC run
    wmMotor30Enabled = true;
    wmMotor30->run(FORWARD);
    wmMotor30->setSpeed(202);
    wmMotor200Enabled = false;
    wmMotor200->run(RELEASE);
    wmMotor200->setSpeed(0);
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
    sMotor3WEnabled = false;
    sMotor3W->run(RELEASE);
    sMotor3W->setSpeed(0);
    sMotorWasteEnabled = false;
    sMotorWaste->run(RELEASE);
    sMotorWaste->setSpeed(0);

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200DrainMillis) { //SDC drain
    wmMotor30Enabled = false;
    wmMotor30->run(RELEASE);
    wmMotor30->setSpeed(0);
    wmMotor200Enabled = false;
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200CleanMillis) { //SDC clean
    wmMotor30Enabled = false;
    wmMotor200Enabled = true;
    wmMotor200->run(FORWARD);
    wmMotor200->setSpeed(150);
    sMotorFilterEnabled = false;
    sMotorFilter->run(RELEASE);
    sMotorFilter->setSpeed(0);
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
    sMotorWasteEnabled = false;

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis) { //1h H2O prime
    wmMotor30Enabled = false;
    wmMotor200Enabled = true;
    wmMotor200->run(FORWARD);
    wmMotor200->setSpeed(150);
    sMotorFilterEnabled = true;
    sMotorFilter->run(FORWARD);
    sMotorFilter->setSpeed(255);
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor30RunH2O1hMillis) { //1h H2O run
    wmMotor30Enabled = true;
    wmMotor30->run(FORWARD);
    wmMotor30->setSpeed(202);
    wmMotor200Enabled = false;
    wmMotor200->run(RELEASE);
    wmMotor200->setSpeed(0);
    sMotorFilterEnabled = false;
    sMotorFilter->run(RELEASE);
    sMotorFilter->setSpeed(0);
    sMotorH2OEnabled = false;
    sMotorDNaseEnabled = false;
    sMotorSDCEnabled = false;
    sMotor3WEnabled = false;
    sMotor3W->run(RELEASE);
    sMotor3W->setSpeed(0);
    sMotorWasteEnabled = false;
    sMotorWaste->run(RELEASE);
    sMotorWaste->setSpeed(0);

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200DrainMillis) { //1h H2O drain
    wmMotor30Enabled = false;//
    wmMotor30->run(RELEASE);
    wmMotor30->setSpeed(0);
    wmMotor200Enabled = false;
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200CleanMillis) { //H2O clean
    wmMotor30Enabled = false;// wmMotor30 off
    wmMotor200Enabled = true;
    wmMotor200->run(FORWARD);
    wmMotor200->setSpeed(150);
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis) { //3h DNase prime
    wmMotor30Enabled = false;
    wmMotor200Enabled = true;
    wmMotor200->run(FORWARD);
    wmMotor200->setSpeed(150);
    sMotorFilterEnabled = true;
    sMotorFilter->run(FORWARD);
    sMotorFilter->setSpeed(255);
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor30DNase3hMillis) { //3h DNase run
    wmMotor30Enabled = true;
    wmMotor30->run(FORWARD);
    wmMotor30->setSpeed(202);
    wmMotor200Enabled = false;
    wmMotor200->run(RELEASE);
    wmMotor200->setSpeed(0);
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
    sMotor3WEnabled = false;
    sMotor3W->run(RELEASE);
    sMotor3W->setSpeed(0);
    sMotorWasteEnabled = false;
    sMotorWaste->run(RELEASE);
    sMotorWaste->setSpeed(0);

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200DrainMillis) { //3h DNase drain
    wmMotor30Enabled = false;
    wmMotor30->run(RELEASE);
    wmMotor30->setSpeed(0);
    wmMotor200Enabled = false;
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200CleanMillis) { //3h DNase clean
    wmMotor30Enabled = false;
    wmMotor200Enabled = true;
    wmMotor200->run(FORWARD);
    wmMotor200->setSpeed(150);
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

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor200PrimeMillis) { //H2O prime
    wmMotor200Enabled = true;// wmMotor200 on
    wmMotor200->run(FORWARD);
    wmMotor200->setSpeed(150);
    wmMotor30Enabled = false;
    sMotorFilterEnabled = true;
    sMotorFilter->run(FORWARD);
    sMotorFilter->setSpeed(255);
    sMotorH2OEnabled = false;
    sMotorDNaseEnabled = false;
    sMotorSDCEnabled = false;
    sMotor3WEnabled = true;
    sMotor3W->run(FORWARD);
    sMotor3W->setSpeed(255);
    sMotorWasteEnabled = true;
    sMotorWaste->run(FORWARD);
    sMotorWaste->setSpeed(255);

    currentMillis = millis();
  }
  startMillis = millis();
  delay(10);
  currentMillis = millis();
  while ((currentMillis - (startMillis + elapsedPause)) < wmMotor30RunH2O72hMillis) { //72h H2O run
    wmMotor30Enabled = true;// wmMotor30 on
    wmMotor30->run(FORWARD);
    wmMotor30->setSpeed(202);
    wmMotor200Enabled = false;
    wmMotor200->run(RELEASE);
    wmMotor200->setSpeed(0);
    sMotorFilterEnabled = false;
    sMotorFilter->run(RELEASE);
    sMotorFilter->setSpeed(0);
    sMotorH2OEnabled = false;
    sMotorDNaseEnabled = false;
    sMotorSDCEnabled = false;
    sMotor3WEnabled = false;
    sMotor3W->run(RELEASE);
    sMotor3W->setSpeed(0);
    sMotorWasteEnabled = false;
    sMotorWaste->run(RELEASE);
    sMotorWaste->setSpeed(0);

    currentMillis = millis();
  }
  currentMillis = millis();
  while (currentMillis > 0) {
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
    sMotorWasteEnabled = true; // NO valve
    sMotorWaste->run(FORWARD);
    sMotorWaste->setSpeed(255);
  }
}
