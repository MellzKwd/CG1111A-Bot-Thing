#include <MeMCore.h>        // Library for handling mBot hardware components

/************* Self-Made Header Files ****************/
#include "Celebrate.h"      // Library for celebratory functions (e.g., playing a tune)
#include "MotorControls.h"  // Library for motor controls
#include "DetectColour.h"   // Library for detecting colours
#include "SensorValues.h"   // Library for to obtain ultrasonic sensor and infrared sensor values
#include "PID.h"            // Library to control PID movements

/************* IR & Ultrasonic Initialisations ****************/
long irHeading = 13.0;
long usThreshold = 17.0; // any ultrasonic values greater than this will utilise IR detector to nudge straight

// Setup and Loop
void setup() {
  pinMode(DECODER_A_PIN, OUTPUT);
  pinMode(DECODER_B_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  float infrared_dist, ultrasonic_dist;
  readSensors(&infrared_dist, &ultrasonic_dist); // Obtain IR and US values from SensorValues.h
  float deviation = infrared_dist - irHeading;

  if (ultrasonic_dist > usThreshold) { // Utilise Infrared Detector, where Ultrasonic is large
      if (deviation < 0.0) nudgeRight();
      else if (deviation > 0.0) nudgeLeft();
      else moveForward();
  } else {
      moveRobot(ultrasonic_dist); // Utilise PID to move the robot otherwise
  }
  
  if (on_black_line()) {
   stopMotor();
   int detectedColor = detectColor(); // Read the colour below robot and execute the required movements
    switch (detectedColor) {
      case RED: turnLeft(); break;
      case GREEN: turnRight(); break;
      case ORANGE: uTurn(); break;
      case PINK: doubleLeftTurn(); break;
      case BLUE: doubleRightTurn(); break;
      case WHITE: celebrate(); break;
     }
  }

}