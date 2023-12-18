#include <rpcWiFi.h>
#include <HTTPClient.h>
#include "Ultrasonic.h"

const char* ssid = "hi mom";
const char* password = "hellothere";

// Pin configuration for the wheels
#define br1 D3
#define br2 D2
#define bl1 D5
#define bl2 D7
#define tr1 D4
#define tr2 D1
#define tl1 D6
#define tl2 D8

#define INTERVAL_MS 100

// Ultrasonic sensor configuration
Ultrasonic ultrasonic(0);

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Configure pins for the wheels as outputs
  pinMode(br1, OUTPUT);
  pinMode(br2, OUTPUT);
  pinMode(bl1, OUTPUT);
  pinMode(bl2, OUTPUT);
  pinMode(tr1, OUTPUT);
  pinMode(tr2, OUTPUT);
  pinMode(tl1, OUTPUT);
  pinMode(tl2, OUTPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting...");
  }

  Serial.print("Connected to the WiFi network with IP: ");
  Serial.println(WiFi.localIP());
  delay(3000);
}

float distance;
String next_action = "";

void loop() {
  // Measure distance using the ultrasonic sensor
  distance = ultrasonic.MeasureInCentimeters();

  if (distance < 15.0) {
    // If an object is detected
    stop(); // Stop the robot

    // Create an HTTPClient object
    HTTPClient http;
    http.setTimeout(20000);

    // Check if WiFi is connected
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("[HTTP] begin...\n");

      // Configure target server and URL for the /object endpoint
      String apiUrl = "http://192.168.12.1:8000/object";
      http.begin(apiUrl.c_str());  // HTTP

      Serial.print("[HTTP] GET...\n");

      // Start connection and send HTTP header
      int httpCode = http.GET();

      // Check the HTTP response
      if (httpCode > 0) {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // Check if the server response is not empty
        if (httpCode != 0) {
          next_action = http.getString();
          // Perform actions based on the server response
          if (next_action == "left") {
            Serial.println("Turning left");
            turnLeft();
            delay(2000);
            stop();
          } else if (next_action == "right") {
            Serial.println("Turning right");
            turnRight();
            delay(2000);
            stop();
          } else {
            turnRight();
            delay(2000);
            Serial.println("Invalid action");
          }
        }
      } else {
        // Print error if HTTP request fails
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      // End the HTTP connection
      http.end();
    }
  } else {
    // If no object is detected, move forward
    Serial.println("Moving forward");
    moveForward();
  }

  // Delay before the next iteration
  delay(INTERVAL_MS);
}

// Functions to control robot movement

void moveForward() {
  bottomLeft(0);
  bottomRight(0);
  topLeft(0);
  topRight(0);
}

void stop() {
  // Stop all wheels
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
  // Control the top-left wheel based on the direction
  if (direction == 0) {
    digitalWrite(tl1, HIGH);
    digitalWrite(tl2, LOW);
  } else {
    digitalWrite(tl1, LOW);
    digitalWrite(tl2, HIGH);
  }
}

void topRight(int direction) {
  // Control the top-right wheel based on the direction
  if (direction == 0) {
    digitalWrite(tr1, HIGH);
    digitalWrite(tr2, LOW);
  } else {
    digitalWrite(tr1, LOW);
    digitalWrite(tr2, HIGH);
  }
}

void bottomLeft(int direction) {
  // Control the bottom-left wheel based on the direction
  if (direction == 0) {
    digitalWrite(bl1, HIGH);
    digitalWrite(bl2, LOW);
  } else {
    digitalWrite(bl1, LOW);
    digitalWrite(bl2, HIGH);
  }
}

void bottomRight(int direction) {
  // Control the bottom-right wheel based on the direction
  if (direction == 0) {
    digitalWrite(br1, HIGH);
    digitalWrite(br2, LOW);
  } else {
    digitalWrite(br1, LOW);
    digitalWrite(br2, HIGH);
  }
}

void turnRight() {
  // Turn the robot to the right
  digitalWrite(tl1, HIGH);
  digitalWrite(tl2, LOW);
  digitalWrite(bl1, HIGH);
  digitalWrite(bl2, LOW);

  digitalWrite(tr1, LOW);
  digitalWrite(tr2, HIGH);
  digitalWrite(br1, LOW);
  digitalWrite(br2, HIGH);
}

void turnLeft() {
  // Turn the robot to the left
  digitalWrite(tl1, LOW);
  digitalWrite(tl2, HIGH);
  digitalWrite(bl1, LOW);
  digitalWrite(bl2, HIGH);

  digitalWrite(tr1, HIGH);
  digitalWrite(tr2, LOW);
  digitalWrite(br1, HIGH);
  digitalWrite(br2, LOW);
}
