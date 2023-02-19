#include <AFMotor.h>

#include <New_Ping.h>

#include <Servo.h>

#define TRIG_PIN A0
#define ECHO_PIN A1
#define MAX_DISTANCE 200
#define MAX_SPEED 190  // sets speed of DC  motors
#define MAX_SPEED_OFFSET 20

//initial motors pin
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

Servo myservo;

boolean goesForward = false;
int distance = 100;
int speedSet = 0;
char command;
//check mode auto
boolean autoMode = false;

void setup() {
  Serial.begin(9600);  //Set the baud rate to your Bluetooth module.

  //setup auto Mode
  myservo.attach(10);
  myservo.write(115);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

void loop() {
  //check every input on time
  if (Serial.available() > 0) {
    command = Serial.read();
    if (!autoMode) {
      Stop();  //stop when uer don't click anything in controler mode
    }

    switch (command) {
      case 'F':
        if (!autoMode) {
          back();
        }
        break;
      case 'B':
        if (!autoMode) {
          forward();
        }
        break;
      case 'L':
        if (!autoMode) {
          left();
        }
        break;
      case 'R':
        if (!autoMode) {
          right();
        }
        break;
      case 'W':  //turn On auto mode
        if (autoMode == false) {
          autoMode = true;
        }
        break;
      case 'w':  //turn Off auto mode
        if (autoMode == true) {
          autoMode = false;
        }
        break;
    }
  }
  //If Auto mode is on
  if (autoMode) {
    autoModef();
  }
  //Try to add Serial.begin to clear the buffer?
  //Serial.begin(9600);

}

void autoModef() {
  int distanceR = 0;
  int distanceL = 0;
  delay(40);
  //Car change ways when encounter obstacles
  if (distance <= 15) {
    moveStop();
    delay(100);
    moveBackward();
    delay(300);
    moveStop();
    delay(200);
    distanceR = lookRight();
    delay(200);
    distanceL = lookLeft();
    delay(200);

    if (distanceR >= distanceL) {
      turnRight();
      moveStop();
    } else {
      turnLeft();
      moveStop();
    }
  } else {
    moveForward();
  }
  distance = readPing();
}

void forward() {
  motor1.setSpeed(255);  //Define maximum velocity
  motor1.run(FORWARD);   //rotate the motor clockwise
  motor2.setSpeed(255);  //Define maximum velocity
  motor2.run(FORWARD);   //rotate the motor clockwise
  motor3.setSpeed(255);  //Define maximum velocity
  motor3.run(FORWARD);   //rotate the motor clockwise
  motor4.setSpeed(255);  //Define maximum velocity
  motor4.run(FORWARD);   //rotate the motor clockwise
}

void back() {
  motor1.setSpeed(255);  //Define maximum velocity
  motor1.run(BACKWARD);  //rotate the motor anti-clockwise
  motor2.setSpeed(255);  //Define maximum velocity
  motor2.run(BACKWARD);  //rotate the motor anti-clockwise
  motor3.setSpeed(255);  //Define maximum velocity
  motor3.run(BACKWARD);  //rotate the motor anti-clockwise
  motor4.setSpeed(255);  //Define maximum velocity
  motor4.run(BACKWARD);  //rotate the motor anti-clockwise
}

void left() {
  motor1.setSpeed(255);  //Define maximum velocity
  motor1.run(BACKWARD);  //rotate the motor anti-clockwise
  motor2.setSpeed(255);  //Define maximum velocity
  motor2.run(BACKWARD);  //rotate the motor anti-clockwise
  motor3.setSpeed(255);  //Define maximum velocity
  motor3.run(FORWARD);   //rotate the motor clockwise
  motor4.setSpeed(255);  //Define maximum velocity
  motor4.run(FORWARD);   //rotate the motor clockwise
}

void right() {
  motor1.setSpeed(255);  //Define maximum velocity
  motor1.run(FORWARD);   //rotate the motor clockwise
  motor2.setSpeed(255);  //Define maximum velocity
  motor2.run(FORWARD);   //rotate the motor clockwise
  motor3.setSpeed(255);  //Define maximum velocity
  motor3.run(BACKWARD);  //rotate the motor anti-clockwise
  motor4.setSpeed(255);  //Define maximum velocity
  motor4.run(BACKWARD);  //rotate the motor anti-clockwise
}

void Stop() {
  motor1.setSpeed(0);   //Define minimum velocity
  motor1.run(RELEASE);  //stop the motor when release the button
  motor2.setSpeed(0);   //Define minimum velocity
  motor2.run(RELEASE);  //rotate the motor clockwise
  motor3.setSpeed(0);   //Define minimum velocity
  motor3.run(RELEASE);  //stop the motor when release the button
  motor4.setSpeed(0);   //Define minimum velocity
  motor4.run(RELEASE);  //stop the motor when release the button
}

//functions auto mode setting
int lookRight() {
  myservo.write(50);
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(115);
  return distance;
}

int lookLeft() {
  myservo.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(115);
  return distance;
  delay(100);
}

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;
  }
  return cm;
}

void moveStop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void moveForward() {

  if (!goesForward) {
    goesForward = true;
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
    for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2)  // slowly bring the speed up to avoid loading down the batteries too quickly
    {
      motor1.setSpeed(speedSet);
      motor2.setSpeed(speedSet);
      motor3.setSpeed(speedSet);
      motor4.setSpeed(speedSet);
      delay(5);
    }
  }
}

void moveBackward() {
  goesForward = false;
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2)  // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
  }
}

void turnRight() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(500);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void turnLeft() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(500);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}
