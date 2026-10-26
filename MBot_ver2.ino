//Please put this outside of any membot files from the old team and/or js make a whole new file and ctrl c ctrl v this in
//thxu

#include "MeMCore.h"

MeDCMotor leftMotor(M1);
MeDCMotor rightMotor(M2);
MeUltrasonicSensor ultrasonic(PORT_3);
MeLineFollower lineFinder(PORT_2);   // change the port if yours differs

const float TOO_CLOSE_CM = 5.0;
const int NORMAL_SPEED = 255;
const int SLOW_SPEED = 100;

const int TURNING_SPEED = 200;       // same as your original code
const int UTURN_TIME_MS = 700;       // calibrate: adjust until it turns exactly 180 degrees
const int CLEAR_LINE_MS = 300;       // drive forward after the turn so it moves off the line

// ---------- colours ----------
// declaration of colours
enum Colour { COLOUR_UNKNOWN, COLOUR_RED, COLOUR_GREEN, COLOUR_BLUE, COLOUR_PINK, COLOUR_BLACK, COLOUR_ORANGE };

void drive(int leftSpeed, int rightSpeed)
{
    leftMotor.run(-leftSpeed);
    rightMotor.run(rightSpeed);
}

void stopMotors()
{
    leftMotor.stop();
    rightMotor.stop();
}
// turn left when it detects RED,, 2 times when it detects PINK
void turnLeft()
{
    leftMotor.run(TURNING_SPEED);
    rightMotor.run(TURNING_SPEED);
    delay(TURNING_TIME_MS);
    stopMotors();
}
// turn right when it detects GREEN,, 2 times when it detects BLUE
void turnRight()
{
    leftMotor.run(-TURNING_SPEED);
    rightMotor.run(-TURNING_SPEED);
    delay(TURNING_TIME_MS);
    stopMotors();
}
// 180 turn when it detects ORANGE
void uTurn()
{
    stopMotors();
    delay(500);                       // short pause at the line

    // Spin on the spot (same direction as your original U-turn)
    leftMotor.run(-TURNING_SPEED - 10);
    rightMotor.run(-TURNING_SPEED - 10);
    delay(UTURN_TIME_MS);

    stopMotors();
    delay(1000);

    // Drive forward without checking the line, so it doesn't see the same line and stop again
    drive(NORMAL_SPEED, NORMAL_SPEED);
    delay(CLEAR_LINE_MS);
}

// --- Smooth left-wall following ---
const int   BASE_SPEED     = 220;   // below 255 so one wheel has room to speed up
const float TARGET_CM      = 8.0;   // distance to hold from the left wall
const float WALL_LOST_CM   = 20.0;  // beyond this = no wall, drive straight
const float KP             = 12.0;  // steering strength (speed per cm of error)
const int   MAX_CORRECTION = 80;    // caps how hard it can steer
const float SMOOTHING      = 0.3;   // 0..1: lower = smoother but slower to react

float filteredDist = TARGET_CM;
bool  hadWall = false;

void followLeftWall()
{
    float raw = ultrasonic.distanceCm();
    if (raw <= 0) return;                 // bad reading, keep current speeds

    if (raw >= WALL_LOST_CM)              // no wall (also catches the 400 "no echo" value)
    {
        hadWall = false;
        drive(BASE_SPEED, BASE_SPEED);
        return;
    }

    if (!hadWall)                         // wall just reappeared: start the filter from the real value
    {
        filteredDist = raw;
        hadWall = true;
    }

    filteredDist = SMOOTHING * raw + (1 - SMOOTHING) * filteredDist;

    float error = TARGET_CM - filteredDist;   // + = too close, - = too far
    int correction = constrain((int)(KP * error), -MAX_CORRECTION, MAX_CORRECTION);

    // too close -> left faster, right slower -> eases away from the wall
    // too far   -> eases back toward it
    drive(constrain(BASE_SPEED + correction, 0, 255),
          constrain(BASE_SPEED - correction, 0, 255));

    Serial.println(filteredDist);
}

void setup()
{
    Serial.begin(9600);
    delay(4000);
}

void loop()
{
    // Black line: stop, U-turn, continue
    int lineState = lineFinder.readSensors();
    Serial.print("Line: ");
    Serial.println(lineState);

    if (lineState != S1_OUT_S2_OUT)
    {
        uTurn();
        return;                       // start the loop again after the turn
    }

       followLeftWall();
    delay(30);
}
