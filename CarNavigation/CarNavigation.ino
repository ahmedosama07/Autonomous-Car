#include "PID.h"
#include "mDriver.h"

// Motor pins
#define AIN1 22
#define BIN1 21
#define AIN2 1
#define BIN2 19
#define PWMA 23
#define PWMB 3

// IR pins
#define extremeLeft 34
#define centerLeft 35
#define center 32
#define centerRight 33
#define extremeRight 25

// Calibration and normal modes' pins
#define calibration 17
#define normal 16

// Indicators
#define calibrationLED 15
#define normalLED 13

// Correction factors for easily modifying motor configuration
// line up with function names like forward.  Value can be 1 or -1
const int correctionA = 1;
const int correctionB = 1;

// Initializing motors.  The library will allow you to initialize as many motors as you have memory for.
Motor leftMotor = Motor(AIN1, AIN2, PWMA, correctionA);
Motor rightMotor = Motor(BIN1, BIN2, PWMB, correctionB);

// Motor speeds
int lsp, rsp;
int lfspeed = 200; // standard speed can be modified later

// PID constants
float Kp = 0;
float Kd = 0;
float Ki = 0;
// set-point variable
int sp = 0;
PID carPID(Kp, Ki, Kd); // PID object

// color map values
int minValues[5], maxValues[5], threshold[5], sensors[5];

void setup()
{
  Serial.begin(9600);
  carPID.setSpeeds(lfspeed);
  pinMode(calibration, INPUT);
  pinMode(normal, INPUT);
  pinMode(calibrationLED, OUTPUT);
  pinMode(normalLED, OUTPUT);
  digitalWrite(normalLED, LOW);
  sensors[0] = extremeLeft;
  sensors[1] = centerLeft;
  sensors[2] = center;
  sensors[3] = centerRight;
  sensors[4] = extremeRight;
}


void loop()
{
  // waiting for calibration
  
  while (!digitalRead(calibration)) {}
  digitalWrite(calibrationLED, HIGH);
  delay(1000);
  carPID.calibrate(leftMotor, rightMotor, minValues, maxValues, threshold, sensors); // calibration mode
  digitalWrite(calibrationLED, LOW);
  // waiting to be set to normal mode
  while (!digitalRead(normal)) {}
  digitalWrite(normalLED, HIGH);
  delay(1000);

  // Normal mode in action
  while (1)
  {
    // Extreme left turn when extremeLeft sensor detects dark region while extremeRight sensor detects white region
    if (analogRead(extremeLeft) < threshold[0] && analogRead(extremeRight) > threshold[4] )
    {
//      lsp = 0; rsp = lfspeed;
//      leftMotor.drive(0);
//      rightMotor.drive(lfspeed);
      left(leftMotor, rightMotor, lfspeed);
    }

    // Extreme right turn when extremeRight sensor detects dark region while extremeLeft sensor detects white region
    else if (analogRead(extremeRight) < threshold[4] && analogRead(extremeLeft) > threshold[0])
    { 
//      lsp = lfspeed; rsp = 0;
//      leftMotor.drive(lfspeed);
//      rightMotor.drive(0);
      right(leftMotor, rightMotor, lfspeed);
    }
    else if (analogRead(center) < threshold[2])
    {
      // arbitrary PID constans will be tuned later
      Kp = 0.0006 * (1000 - analogRead(center));
      Kd = 10 * Kp;
      Ki = 0.0001;
      carPID.setConstants(Kp, Ki, Kd);
      sp = (analogRead(centerLeft) - analogRead(centerRight));
      carPID.setSetpoint(sp);
      carPID.linefollow(leftMotor, rightMotor, lsp, rsp);
    }
  }
}
