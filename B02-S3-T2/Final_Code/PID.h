// PID.h - Obtain PID values based on ultrasonic distance recorded

#ifndef PID_h
#define PID_h


/************* PID Control Constants ****************/
float Kp = 16;           // Proportional gain for PID
float Ki = 0.1587;       // Integral gain for PID
float Kd = 1.95;         // Derivative gain for PID


/************* PID Control Initialisations ****************/
float targetHeading = 8;   // Desired heading angle or target distance
float error = 0;           // Difference between target and current heading
float previousError = 0;   // Error from the previous cycle
float integral = 0;        // Sum of all previous errors for integral term
float maxIntegral = 100.0; // Maximum limit for integral to prevent "integral windup"
float derivative = 0;      // Rate of change of error

/************* Functions to move robot using PID control ****************/
void moveRobot(float currentHeading) {
  error = targetHeading - currentHeading;       // Error as difference from target
  derivative = error - previousError;           // Derivative as error change rate
  integral += error;                            // Accumulate error for integral term

  // Limit integral to prevent integral windup
  if (integral > maxIntegral) integral = maxIntegral;
  if (integral < -maxIntegral) integral = -maxIntegral;

  // Calculate correction using PID formula
  float correction = (Kp * error) + (Ki * integral) + (Kd * derivative);

  // Adjust motor speeds based on correction
  motor1Speed = base1Speed - correction;
  motor2Speed = base2Speed + correction;

  // Apply speeds to motors
  motor1.run(-motor1Speed); // Inverse direction for motor1
  motor2.run(motor2Speed);  // Forward direction for motor2

  previousError = error;    // Store current error for next loop iteration
  delay(10);
}

#endif // PID.h