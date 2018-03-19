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

// These might need to be tuned for different motor types.
int REVERSE_SPEED = 200; // 0 is stopped, 400 is full speed
int TURN_SPEED = 150;
int FORWARD_SPEED = 300;
int REVERSE_DURATION = 200;  // ms
int TURN_DURATION = 400;  // ms

#define ACK 0x01
#define NAK 0x0f

//Bluetooth del
#include <SoftwareSerial.h>

#define BT_TX_PIN 10 //Connect this to pin RXD on the BT unit
#define BT_RX_PIN 11 //Connect this to pin TXD on the BT unit
SoftwareSerial btSerial(BT_TX_PIN, BT_RX_PIN);
String btBuffer = "";

#define NUM_SENSORS 6
unsigned int lineSensorValues[NUM_SENSORS];

void waitForButtonAndCountDown()
{
  // button.waitForButton();
}

void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);

  waitForButtonAndCountDown();  // No good. REDO!
  lineSensors.init();
}


void readCommand() {
  String input = btBuffer;
  
  while (btSerial.available()) {
    char c = btSerial.read();
    input += c;
  }

  if (input.indexOf("#") == -1) {
    btBuffer = input;
    return;
  } else {
    btBuffer = "";
  }

  String cmd = input.substring(0, input.indexOf(':'));
  String val = input.substring(input.indexOf(':') + 1, input.length() - 1);
  int BTvalue = val.toInt();

  Serial.println(cmd + " " + val);

  if (cmd.equals("HI")) {
    btSerial.write(ACK);
    Serial.println("BT connection established");
  } else if (cmd.equals("RS")) {
    REVERSE_SPEED = BTvalue;
    btSerial.write(ACK);
  } else if (cmd.equals("TS")) {
    TURN_SPEED = BTvalue;
    btSerial.write(ACK);
  } else if (cmd.equals("FS")) {
    FORWARD_SPEED = BTvalue;
    btSerial.write(ACK);
  } else if (cmd.equals("RD")) {
    REVERSE_DURATION = BTvalue;
    btSerial.write(ACK);
  } else if (cmd.equals("TD")) {
    TURN_DURATION = BTvalue;
    btSerial.write(ACK);
  } else {
    btSerial.write(NAK);
  }
  
  printParameters();
}


void printParameters() {
  Serial.println("--------------");
  Serial.println("RS " + String(REVERSE_SPEED));
  Serial.println("FS " + String(FORWARD_SPEED));
  Serial.println("TS " + String(TURN_SPEED));
  Serial.println("RD " + String(REVERSE_DURATION));
  Serial.println("TD " + String(TURN_DURATION));
  Serial.println("--------------\n");
}


void loop() {
  if (btSerial.available()) {
    readCommand();
  }

/*
  if (button.isPressed()) {
    motors.setSpeeds(0, 0);
    button.waitForRelease();
    waitForButtonAndCountDown();
  }
*/
/*
  lineSensors.read(lineSensorValues);

  if (lineSensorValues[0] < QTR_THRESHOLD) {
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  } else if (lineSensorValues[NUM_SENSORS - 1] < QTR_THRESHOLD) {
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  } else {
    // Otherwise, go straight.
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
*/
}





