// DetectColour.h - Define functions to obtain colour detected by the LDR

#ifndef DetectColour_h
#define DetectColour_h


/************* Pin Numbers ****************/
#define DECODER_A_PIN 11 // Decoder pin A for color sensor
#define DECODER_B_PIN 12 // Decoder pin B for color sensor
#define LDR A1           // Pin for the Light Dependent Resistor (LDR) sensor

/************* Colour Definitions ****************/
#define RED 1
#define GREEN 2
#define ORANGE 3
#define PINK 4
#define BLUE 5
#define WHITE 6

/************* Stabilisation Delays ****************/
#define RGBWait 70      // Delay for RGB color stabilization in milliseconds
#define LDRWait 100      // Delay for LDR reading stabilization in milliseconds


/************* Color Detection Reference Arrays ****************/
float whiteArray[] = {198, 255, 257};   // Reference values for white
float blackArray[] = {629, 821, 778};   // Reference values for black
float greyDiff[] = {120, 95, 75};       // Differences for normalization
float redArray[] = {170, 60, 34};       // Reference values for red
float greenArray[] = {60, 189, 97};     // Reference values for green
float orangeArray[] = {165, 120, 37};   // Reference values for orange
float pinkArray[] = {191, 196, 184};    // Reference values for pink
float blueArray[] = {72, 190, 220};     // Reference values for blue


/************* Functions to Detect Colours ****************/
// Black Line Detection Function
bool on_black_line() {
  int sensorState = lineFinder.readSensors();
  // Return true if the sensor detects a black line (specific sensor states)
  return (sensorState == S1_IN_S2_IN || sensorState == S1_OUT_S2_IN || sensorState == S1_IN_S2_OUT);
}

// Helper function to get an average reading from LDR sensor
int getAvgReading(int times) {
  int total = 0;
  for (int i = 0; i < times; i++) {
    total += analogRead(LDR);        // Read LDR value and add to total
    delay(LDRWait);                  // Delay between readings for stabilization
  }
  return total / times;              // Return the average reading
}

// Function to get the average red color reading
int find_RED() {
  digitalWrite(DECODER_A_PIN, LOW);  // Set decoder to read red component
  digitalWrite(DECODER_B_PIN, LOW);
  delay(RGBWait);                    // Wait for stabilization
  return getAvgReading(5);           // Return average reading over 5 samples
}

// Function to get the average green color reading
int find_GREEN() {
  digitalWrite(DECODER_A_PIN, HIGH);  // Set decoder to read green component
  digitalWrite(DECODER_B_PIN, LOW);
  delay(RGBWait);                     // Wait for stabilization
  return getAvgReading(5);            // Return average reading over 5 samples
}

// Function to get the average blue color reading
int find_BLUE() {
  digitalWrite(DECODER_A_PIN, LOW);   // Set decoder to read blue component
  digitalWrite(DECODER_B_PIN, HIGH);
  delay(RGBWait);                     // Wait for stabilization
  return getAvgReading(5);            // Return average reading over 5 samples
}

// Function to normalize the color readings based on black and grey calibration arrays
void readNormalizedColor(int color[]) {
  color[0] = (find_RED() - blackArray[0]) / (greyDiff[0]) * 255;   // Normalize red value
  color[1] = (find_GREEN() - blackArray[1]) / (greyDiff[1]) * 255; // Normalize green value
  color[2] = (find_BLUE() - blackArray[2]) / (greyDiff[2]) * 255;  // Normalize blue value
  return color;  // Return the normalized color array
}

// Function to calculate Euclidean distance between two colors in 3D color space
float distance(int x1, int y1, int z1, int x2, int y2, int z2) {
  return sqrt(square(x2 - x1) + square(y2 - y1) + square(z2 - z1));
}


// Function to detect the closest matching color by comparing current color with known colors
int detectColor() {
  int currentColor[3];                      // Array to hold current normalized color values
  readNormalizedColor(currentColor);         // Get normalized color values
  
  // Print current color values for debugging
  Serial.println(currentColor[0]);
  Serial.println(currentColor[1]);
  Serial.println(currentColor[2]);

  // Calculate distances between current color and each predefined color
  float distances[] = {
    distance(redArray[0], redArray[1], redArray[2], currentColor[0], currentColor[1], currentColor[2]),
    distance(greenArray[0], greenArray[1], greenArray[2], currentColor[0], currentColor[1], currentColor[2]),
    distance(orangeArray[0], orangeArray[1], orangeArray[2], currentColor[0], currentColor[1], currentColor[2]),
    distance(pinkArray[0], pinkArray[1], pinkArray[2], currentColor[0], currentColor[1], currentColor[2]),
    distance(blueArray[0], blueArray[1], blueArray[2], currentColor[0], currentColor[1], currentColor[2]),
    distance(whiteArray[0], whiteArray[1], whiteArray[2], currentColor[0], currentColor[1], currentColor[2])
  };

  int colors[] = {RED, GREEN, ORANGE, PINK, BLUE, WHITE};  // Array representing color codes

  // Find the color with the minimum distance
  int minColor = colors[0];      // Initialize to minimum distance and its corresponding minimum colour
  float minDist = distances[0];  
  for (int i = 1; i < 6; i++) {  
    if (distances[i] < minDist) {  // Check if current distance is smaller and update accordingly
      minDist = distances[i];      
      minColor = colors[i];        
    }
  }
  return minColor;
}

#endif // DetectColour.h