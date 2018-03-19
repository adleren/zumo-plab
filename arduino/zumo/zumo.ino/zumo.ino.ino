#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <ZumoReflectanceSensorArray.h>
#include <QTRSensors.h>

ZumoMotors motors;
ZumoReflectanceSensorArray lineSensors;
Pushbutton button(ZUMO_BUTTON);

// This might need to be tuned for different lighting conditions,
// surfaces, etc.
#define QTR_THRESHOLD     1000  // microseconds
const int MAX_SPEED = 400;
int leftMotorSpeed = 100;
int rightMotorSpeed = 100;

// These might need to be tuned for different motor types.
#define REVERSE0_SPEED     200  // 0 is stopped, 400 is full speed
int TURN_SPEED = 150;
int FORWARD_SPEED = 300;
#define REVERSE_DURATION  200  // ms
#define TURN_DURATION     400  // ms


//Bluetooth del
#include <SoftwareSerial.h>

#define rxPin2 //Connect this to pin RXD on the BT unit
#define txPin3 //Connect this to pin TXD on the BT unit

SoftwareSerial btSerial(txPin, rxPin);


#define NUM_SENSORS 6
unsigned int lineSensorValues[NUM_SENSORS];

void waitForButtonAndCountDown()
{
  button.waitForButton();
}

void setup() {
   waitForButtonAndCountDown();
   lineSensors.init();

   //Bluetooth
   Serial.begin(9600);
   btSerial.begin(9600);
}

void readCommand(){
  while (btSerial.available()) {
      char c = btSerial.read();
      Serial.write(c);
    };
    while (Serial.available()) {
      char c = Serial.read();
      btSerial.write(c);
    };    
}


void loop() {
  //Bluetooth del
  readCommand();

  //Zumo del
  if (button.isPressed())
  {
    // If button is pressed, stop and wait for another press to
    // go again.
    motors.setSpeeds(0, 0);
    button.waitForRelease();
    waitForButtonAndCountDown();
  }

  lineSensors.read(lineSensorValues);

  if (lineSensorValues[0] < QTR_THRESHOLD) 
  {
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else if (lineSensorValues[NUM_SENSORS - 1] < QTR_THRESHOLD)
  {
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else
  {
    // Otherwise, go straight.
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
}





