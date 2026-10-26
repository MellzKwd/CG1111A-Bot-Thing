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

    // Wall check, as before
    float distance = ultrasonic.distanceCm();
    Serial.println(distance);

    if (distance < TOO_CLOSE_CM)
    {
        drive(NORMAL_SPEED, SLOW_SPEED);
    }
    else
    {
        drive(NORMAL_SPEED, NORMAL_SPEED);
    }

    delay(30);
}