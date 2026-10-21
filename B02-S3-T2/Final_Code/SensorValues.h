// SensorValues.h - Define functions to obtain Ultrasonic and Infrared Values

#ifndef SensorValues_h
#define SensorValues_h


/************* Pin Number ****************/
#define IR_PIN A0      // Analog pin connected to IR sensor

/************* Sensor Initialisation ****************/
MeUltrasonicSensor ultraSensor(PORT_3); // Ultrasonic sensor connected to Port 3

/************* Functions to Obtain Ultrasonic and Infrared Values ****************/
float readUltrasonic() {
  int distance_measured = ultraSensor.distanceCm();  // Get distance measurement in cm
  delay(10);                           // Delay for stability in readings
  return distance_measured;
}

int findAmbient(){
  digitalWrite(DECODER_A_PIN, HIGH);  // Set decoder pins to read infrared signal
  digitalWrite(DECODER_B_PIN, LOW);
  delay(10);
  int ambientValue = analogRead(IR_PIN);
  delay(10);
  return ambientValue;
}

// Function to read and calibrate distance from infrared sensor
float readInfrared() {
  digitalWrite(DECODER_A_PIN, HIGH);  // Set decoder pins to read infrared signal
  digitalWrite(DECODER_B_PIN, HIGH);
  delay(10);  
  int sensorValue = analogRead(IR_PIN);  // Get raw analog value from IR sensor
  delay(10);                           // Delay for stability in readings
  float a = 9901.2;                    // Calibration constant (determined experimentally)
  float b = -9.3664;                    // Calibration constant (determined experimentally)

  int ambientValue = findAmbient();

  int noAmbient = sensorValue - ambientValue;
  float calibrated_distance = (a / noAmbient) + b;  // Calibrate distance using formula
  return calibrated_distance;
}

// Function to read and store both infrared and ultrasonic distances
void readSensors(float* infrared_dist, float* ultrasonic_dist) {
  *infrared_dist = readInfrared();   // Store infrared distance in passed variable
  *ultrasonic_dist = readUltrasonic(); // Store ultrasonic distance in passed variable
}


#endif // SensorValues.h