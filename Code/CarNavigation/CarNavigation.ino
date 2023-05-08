#include "PID.h"
#include "mDriver.h"

#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
String message = "";

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
  SerialBT.begin("Hossam Hassan");
  Serial.println("Car Started! Ready to pair...");
  
  carPID.setSpeeds(lfspeed);
  carPID.setConstrains(0, 255);

  Kp = 5;
  Kd = 1;
  Ki = 0.001;
  carPID.setConstants(Kp, Ki, Kd);
  
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
  while ((!digitalRead(calibration)) && (message != "0")) {
      if (SerialBT.available()){
        char incomingChar = SerialBT.read();
        if (incomingChar != '\n'){
          message += String(incomingChar);
        }
        else{
          message = "";
        }
      }
    }
  SerialBT.println("Calibrating...");
  Serial.println("Calibrating...");
  digitalWrite(calibrationLED, HIGH);
  digitalWrite(normalLED, LOW);
  delay(1000);
  carPID.calibrate(leftMotor, rightMotor, minValues, maxValues, threshold, sensors); // calibration mode
  digitalWrite(calibrationLED, LOW);

  SerialBT.print("Thresholds\n");
  SerialBT.print("Extreme left: ");
  SerialBT.print(threshold[0]);
  SerialBT.print("\n");
  SerialBT.print("Center left: ");
  SerialBT.print(threshold[1]);
  SerialBT.print("\n");
  SerialBT.print("Center: ");
  SerialBT.print(threshold[2]);
  SerialBT.print("\n");
  SerialBT.print("Center right: ");
  SerialBT.print(threshold[3]);
  SerialBT.print("\n");
  SerialBT.print("Extreme right: ");
  SerialBT.print(threshold[4]);
  SerialBT.println("\n");
  
  // waiting to be set to normal mode
  while ((!digitalRead(normal)) && (message != "1")) {
    if (SerialBT.available()){
        char incomingChar = SerialBT.read();
        if (incomingChar != '\n'){
          message += String(incomingChar);
        }
        else{
          message = "";
        }
      }
    }
  SerialBT.println("Running...brrrr\n");
  Serial.println("Running...brrrr\n");
  digitalWrite(calibrationLED, LOW);
  digitalWrite(normalLED, HIGH);
  delay(1000);

  // Normal mode in action
  while (1)
  {
    if (SerialBT.available()){
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n'){
      message += String(incomingChar);
    }
    else{
      message = "";
    }
  }
    if(digitalRead(normal) || message == "1")
    {
      SerialBT.println("Stopping...");
      stop(leftMotor, rightMotor);
      digitalWrite(calibrationLED, LOW);
      digitalWrite(normalLED, LOW);
      break;
    }
    // Extreme left turn when extremeLeft sensor detects dark region while extremeRight sensor detects white region
    if (analogRead(extremeLeft) > threshold[0] && analogRead(extremeRight) < threshold[4] )
    {
      
      lsp = 0; rsp = lfspeed;
      left(leftMotor, rightMotor, lfspeed);
    }

    // Extreme right turn when extremeRight sensor detects dark region while extremeLeft sensor detects white region
    else if (analogRead(extremeRight) > threshold[4] && analogRead(extremeLeft) < threshold[0])
    { 
      
      lsp = lfspeed; rsp = 0;
      right(leftMotor, rightMotor, lfspeed);
    }
    else if (analogRead(center) > threshold[2])
    {
      sp = (analogRead(centerLeft) - analogRead(centerRight));
      carPID.setSetpoint(sp);
      carPID.linefollow(leftMotor, rightMotor, lsp, rsp);
    }
  }
}
