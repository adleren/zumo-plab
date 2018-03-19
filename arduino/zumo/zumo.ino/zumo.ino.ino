#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <ZumoReflectanceSensorArray.h>
#include <QTRSensors.h>

ZumoMotors motors;
ZumoReflectanceSensorArray lineSensors;
Pushbutton button(ZUMO_BUTTON);

// This might need to be tuned for different lighting conditions,
// surfaces, etc.
int QTR_THRESHOLD = 1000;  // microseconds
const int MAX_SPEED = 400;
int leftMotorSpeed = 100;
int rightMotorSpeed = 100;
String input;

// These might need to be tuned for different motor types.
int REVERSE_SPEED = 200; // 0 is stopped, 400 is full speed
int TURN_SPEED = 150;
int FORWARD_SPEED = 300;
int REVERSE_DURATION = 200;  // ms
int TURN_DURATION = 400;  // ms


//Bluetooth del
#include <SoftwareSerial.h>

#define rxPin 2 //Connect this to pin RXD on the BT unit
#define txPin 3 //Connect this to pin TXD on the BT unit

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

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void readCommand(){
  while (btSerial.available()) {
      char c = btSerial.read();
      Serial.write(c);
      // SS:200#
      while(c!='#'){
        input += c;
      }
      String whatToDo = getValue(input, ':', 0);
      String value = getValue(input, ':', 1);
      int BTvalue = value.toInt(); 
        
      if(whatToDo.equals("RS")){
        REVERSE_SPEED = BTvalue;
        btSerial.write(0x01); 
      }
      else if(whatToDo.equals("TS")){
        TURN_SPEED = BTvalue; 
        btSerial.write(0x01); 
      }
      else if(whatToDo.equals("FS")){
        FORWARD_SPEED = BTvalue; 
        btSerial.write(0x01); 
      }
      else if(whatToDo.equals("RD")){
        REVERSE_DURATION = BTvalue; 
        btSerial.write(0x01); 
      }
      else if(whatToDo.equals("TD")){
        TURN_DURATION = BTvalue; 
        btSerial.write(0x01); 
      }
      else{
        btSerial.write(0x0f); 
      }
  }
  while (Serial.available()) {
     char c = Serial.read();
     btSerial.write(c);
  }    
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





