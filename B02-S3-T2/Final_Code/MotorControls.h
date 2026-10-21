// MotorControls.h - Define functions to Control Motors for turning and nudging

#ifndef MotorControls_h
#define MotorControls_h


/************* Motor Initialisation ****************/
MeDCMotor motor1(M1);                   
MeDCMotor motor2(M2);                   
MeLineFollower lineFinder(PORT_2);


/************* Motor Speed Settings ****************/
long base1Speed = 170;         // Base speed for motor 1 (Higher value, to ensure robot is moving in a straight manner)
long base2Speed = 150;         // Base speed for motor 2
long motor1Speed, motor2Speed; // Variables to hold adjusted motor speeds


/************* Timing Constants ****************/
long right_left_dur = 362; // Duration for left/right turn in milliseconds
long uturn_dur = 700;      // Duration for U-turn in milliseconds
long speed_gain = 80;      // Speed adjustment for turns


/************* Functions to Execute Turns ****************/
// Function to stop both motors
void stopMotor() {
  motor1.stop();  
  motor2.stop();
}

// Function to move the robot forward at base speed
void moveForward() {
  motor1.run(-base1Speed); 
  motor2.run(base2Speed);
}

// Function to turn the robot right
void turnRight() {
  motor1.run(-base1Speed - speed_gain); // Motor 1 speed increased for sharper turn
  motor2.run(-base2Speed);              
  delay(right_left_dur);                // Delay to control turn duration
}

// Function to turn the robot left
void turnLeft() {
  motor1.run(base1Speed);               
  motor2.run(base2Speed + speed_gain);  // Motor 2 speed increased for sharper turn
  delay(right_left_dur);                // Delay to control turn duration
}

// Function to perform a U-turn
void uTurn() {
  motor1.run(base1Speed + speed_gain);  // Motor 1 runs forward with increased speed
  motor2.run(base2Speed);
  delay(uturn_dur);                     // Delay to complete a full U-turn
}

// Function to slightly adjust the robot to the left
void nudgeLeft() {
  motor1.run(-base1Speed + 20);         // Slightly reduce Motor 1 reverse speed
  motor2.run(base2Speed + 30);          // Slightly increase Motor 2 speed
  delay(100);
}

// Function to slightly adjust the robot to the right
void nudgeRight() {
  motor1.run(-base1Speed - 20);         // Slightly increase Motor 1 reverse speed
  motor2.run(base2Speed - 10);          // Slightly reduce Motor 2 speed
  delay(100);
}

// Function to perform a double left turn sequence
void doubleLeftTurn() {
  turnLeft();       // Perform first left turn
  moveForward();    // Move forward briefly to reposition
  delay(1150);      // Wait for movement stabilization
  stopMotor();      // Stop motors to prevent overshoot
  turnLeft();       // Perform second left turn
}

// Function to perform a double right turn sequence
void doubleRightTurn() {
  turnRight();      // Perform first right turn
  moveForward();    // Move forward briefly to reposition
  delay(1000);      // Wait for movement stabilization
  stopMotor();      // Stop motors to prevent overshoot
  turnRight();      // Perform second right turn
}

#endif // MotorControls_h