#include <SoftwareSerial.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <ZumoReflectanceSensorArray.h>
#include <QTRSensors.h>

#define ACK 0x01
#define NAK 0x0f

#define BT_TX_PIN 19
#define BT_RX_PIN 18

#define LISTEN_LED 15

#define NUM_SENSORS 6

#define OPERATE 1
#define LISTEN 2
int state = LISTEN;

unsigned int lineSensorValues[NUM_SENSORS];

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


SoftwareSerial btSerial(BT_TX_PIN, BT_RX_PIN);
String btBuffer = "";


void setup() {
  int state = LISTEN;
  
  lineSensors.init();

  pinMode(LISTEN_LED, OUTPUT);
  digitalWrite(LISTEN_LED, HIGH);
  
  Serial.begin(9600);
  btSerial.begin(9600);
}


void loop() {

  if (button.isPressed()) {
    motors.setSpeeds(0, 0);
    button.waitForRelease();
    if (state == OPERATE) {
      state = LISTEN;
      digitalWrite(LISTEN_LED, HIGH);
    } else if (state == LISTEN) {
      state = OPERATE;
      digitalWrite(LISTEN_LED, LOW);
    }
  }

  if (state == OPERATE) {
    executeStrategy();
  } else if (state == LISTEN) {
    if (btSerial.available() > 0) {
      readCommand();
    }
  }
}


void waitForButtonAndCountDown() {
  button.waitForButton();
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
  } else if (cmd.equals("ST")) {
     if (setStrategy(BTvalue)) {
      btSerial.write(ACK);
     } else {
      btSerial.write(NAK);
     }
  } else {
    btSerial.write(NAK);
  }
  
  printParameters();
}


boolean setStrategy(int strategy) {
  switch(strategy) {
  
  // Default
  case 1:
    REVERSE_SPEED = 200;
    TURN_SPEED = 150;
    FORWARD_SPEED = 300;
    REVERSE_DURATION = 200;
    TURN_DURATION = 400;

    return true;

  // Full pupp
  case 2:
    REVERSE_SPEED = 400;
    TURN_SPEED = 400;
    FORWARD_SPEED = 400;
    REVERSE_DURATION = 500;
    TURN_DURATION = 500;
    
    return true;

  // Idk...
  case 3:
    REVERSE_SPEED = 400;
    TURN_SPEED = 200;
    FORWARD_SPEED = 150;
    REVERSE_DURATION = 250;
    TURN_DURATION = 150;
    
    return true;

  // Slow poke
  case 4:
    REVERSE_SPEED = 50;
    TURN_SPEED = 200;
    FORWARD_SPEED = 50;
    REVERSE_DURATION = 200;
    TURN_DURATION = 500;
    
    return true;
  }
  return false;
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


void executeStrategy() {
  // Read line sensors
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
}









