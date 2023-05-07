#include "PID.h"
#include "mDriver.h"

#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

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
const int correctionA = -1;
const int correctionB = -1;

// Initializing motors.  The library will allow you to initialize as many motors as you have memory for.
Motor leftMotor = Motor(AIN1, AIN2, PWMA, correctionA);
Motor rightMotor = Motor(BIN1, BIN2, PWMB, correctionB);

// Motor speeds
int lsp, rsp;
int lfspeed = 255; // standard speed can be modified later

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
  Serial.begin(115200);
  SerialBT.begin();
  Serial.println("Car Started! Ready to pair...");
  
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

  for(int i = 0; i < 5; i++)
  {
    pinMode(sensors[i], INPUT);
  }
}


void loop()
{
  // waiting for calibration
  
  while (!digitalRead(calibration)) {}
  SerialBT.print("Calibrating...\n");
  digitalWrite(calibrationLED, HIGH);
  digitalWrite(normalLED, LOW);
  delay(1000);
  carPID.calibrate(leftMotor, rightMotor, minValues, maxValues, threshold, sensors); // calibration mode
  digitalWrite(calibrationLED, LOW);
  
  // waiting to be set to normal mode
  while (!digitalRead(normal)) {}
  SerialBT.print("Running...brrrr\n");
  digitalWrite(calibrationLED, LOW);
  digitalWrite(normalLED, HIGH);
  delay(1000);

  // Normal mode in action
  while (1)
  {
    if(digitalRead(normal))
    {
      SerialBT.print("Stopping...\n");
      stop(leftMotor, rightMotor);
      digitalWrite(calibrationLED, LOW);
      digitalWrite(normalLED, LOW);
      break;
    }
    /*for ( int i = 0; i < 5; i++)
    {
      Serial.print(analogRead(sensors[i]));
      Serial.print("   ");
    }
  Serial.println();*/
    // Extreme left turn when extremeLeft sensor detects dark region while extremeRight sensor detects white region
    if (analogRead(center) > threshold[2])
    {
      // arbitrary PID constans will be tuned later
      //forward(leftMotor, rightMotor);
      Kp = 5;
      Kd = 1;
      Ki = 0.001;
      //Serial.print("center\t");
      //Serial.println(Kp);
      carPID.setConstants(Kp, Ki, Kd);
      sp = (analogRead(centerLeft) - analogRead(centerRight));
      //Serial.print("error\t");
      //Serial.println(sp);
      carPID.setSetpoint(sp);
      carPID.linefollow(leftMotor, rightMotor, lsp, rsp);
    }
    // Extreme left turn when extremeLeft sensor detects dark region while extremeRight sensor detects white region
    else if (analogRead(extremeLeft) > threshold[0] && analogRead(extremeRight) < threshold[4] )
    {
      //Serial.println("left");
      lsp = 0; rsp = lfspeed;
      // leftMotor.drive(0);
      // rightMotor.drive(rsp);
      //SerialBT.println("left");
      left(leftMotor, rightMotor, lfspeed);
    }

    // Extreme right turn when extremeRight sensor detects dark region while extremeLeft sensor detects white region
    else if (analogRead(extremeRight) > threshold[4] && analogRead(extremeLeft) < threshold[0])
    { 
      //Serial.println("right");
      lsp = lfspeed; rsp = 0;
      // leftMotor.drive(lsp);
      // rightMotor.drive(0);
      //SerialBT.println("right");
      right(leftMotor, rightMotor, lfspeed);
    }
  }
}
