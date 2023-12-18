#include "Ultrasonic.h"

// Define the wheels pins
#define br1 D3
#define br2 D2
#define bl1 D5
#define bl2 D7
#define tr1 D4
#define tr2 D1
#define tl1 D6
#define tl2 D8

Ultrasonic ultrasonic(0);

void setup() {
  Serial.begin(115200);

  pinMode(br1, OUTPUT);
  pinMode(br2, OUTPUT);
  pinMode(bl1, OUTPUT);
  pinMode(bl2, OUTPUT);
  pinMode(tr1, OUTPUT);
  pinMode(tr2, OUTPUT);
  pinMode(tl1, OUTPUT);
  pinMode(tl2, OUTPUT);
}

float distance;
void loop() {
  distance = ultrasonic.MeasureInCentimeters();
  Serial.println(distance);

  if (distance <= 15) { 
    stop();
    delay(1000);
    turnRight(); 
    delay(2000);
    stop();
    delay(1000);
  } else {
    moveForward();
  }
  delay(10);
}

void moveForward() {
  bottomLeft(0);
  bottomRight(0);
  topLeft(0);
  topRight(0);
}

void stop() {
  digitalWrite(tl1, LOW);
  digitalWrite(tl2, LOW);
  digitalWrite(tr1, LOW);
  digitalWrite(tr2, LOW);
  digitalWrite(br1, LOW);
  digitalWrite(br2, LOW);
  digitalWrite(bl1, LOW);
  digitalWrite(bl2, LOW);
}

void topLeft(int direction) {
  if (direction == 0) {
    digitalWrite(tl1, HIGH);
    digitalWrite(tl2, LOW);
  } else {
    digitalWrite(tl1, LOW);
    digitalWrite(tl2, HIGH);
  }
}

void topRight(int direction) {
  if (direction == 0) {
    digitalWrite(tr1, HIGH);
    digitalWrite(tr2, LOW);
  } else {
    digitalWrite(tr1, LOW);
    digitalWrite(tr2, HIGH);
  }
}

void bottomLeft(int direction) {
  if (direction == 0) {
    digitalWrite(bl1, HIGH);
    digitalWrite(bl2, LOW);
  } else {
    digitalWrite(bl1, LOW);
    digitalWrite(bl2, HIGH);
  }
}

void bottomRight(int direction) {
  if (direction == 0) {
    digitalWrite(br1, HIGH);
    digitalWrite(br2, LOW);
  } else {
    digitalWrite(br1, LOW);
    digitalWrite(br2, HIGH);
  }
}

void turnRight() {
// Make the left wheels (top left and bottom left) to move forward 
  digitalWrite(tl1, HIGH);
  digitalWrite(tl2, LOW);
  digitalWrite(bl1, HIGH);
  digitalWrite(bl2, LOW);

// Make the right wheels (top right and bottom right) to stop
  digitalWrite(tr1, LOW);
  digitalWrite(tr2, LOW);
  digitalWrite(br1, LOW);
  digitalWrite(br2, LOW);
}

