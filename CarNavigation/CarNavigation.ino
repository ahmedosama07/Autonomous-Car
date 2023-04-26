#include "PID.h"
#include "mDriver.h"

// Motor pins
#define AIN1 4
#define BIN1 6
#define AIN2 3
#define BIN2 7
#define PWMA 9
#define PWMB 10

// IR pins
#define extremeLeft 1
#define centerLeft 2
#define center 3
#define centerRight 4
#define extremeRight 5

// Calibration and normal modes' pins
#define calibration 11
#define normal 12

// Correction factors for easily modifying motor configuration
// line up with function names like forward.  Value can be 1 or -1
const int correctionA = 1;
const int correctionB = 1;

// Initializing motors.  The library will allow you to initialize as many motors as you have memory for.
Motor leftMotor = Motor(AIN1, AIN2, PWMA, correctionA);
Motor rightMotor = Motor(BIN1, BIN2, PWMB, correctionB);

// Motor speeds
int lsp, rsp;
int lfspeed = 200; // standard speed can be nodified later

// PID constants
float Kp = 0;
float Kd = 0;
float Ki = 0;
// set-point variable
int sp = 0;
PID carPID(Kp, Ki, Kd); // PID object

// color map values
int minValues[6], maxValues[6], threshold[6];

void setup()
{
  Serial.begin(9600);
  carPID.setSpeeds(lfspeed);
  pinMode(calibration, INPUT_PULLUP);
  pinMode(normal, INPUT_PULLUP);
}


void loop()
{
  // waiting for calibration
  while (digitalRead(calibration)) {}
  delay(1000);
  carPID.calibrate(leftMotor, rightMotor, minValues, maxValues, threshold); // calibration mode
  // waiting to be set to normal mode
  while (digitalRead(normal)) {}
  delay(1000);

  // Normal mode in action
  while (1)
  {
    // Extreme left turn when extremeLeft sensor detects dark region while extremeRight sensor detects white region
    if (analogRead(extremeLeft) > threshold[1] && analogRead(extremeRight) < threshold[5] )
    {
      lsp = 0; rsp = lfspeed;
      leftMotor.drive(0);
      rightMotor.drive(lfspeed);
    }

    // Extreme right turn when extremeRight sensor detects dark region while extremeLeft sensor detects white region
    else if (analogRead(extremeRight) > threshold[5] && analogRead(extremeLeft) < threshold[1])
    { lsp = lfspeed; rsp = 0;
      leftMotor.drive(lfspeed);
      rightMotor.drive(0);
    }
    else if (analogRead(center) > threshold[3])
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
