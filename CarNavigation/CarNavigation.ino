#include "PID.h"
#include "mDriver.h"

#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

const char *pin = "1234"; // Change this to more secure PIN.

String device_name = "ESP32-BT-Slave";

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
  Serial.begin(115200);
  SerialBT.begin(device_name); //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());

  #ifdef USE_PIN
    SerialBT.setPin(pin);
    Serial.println("Using PIN");
  #endif
  
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
    if(digitalRead(normal)) break;
  for ( int i = 0; i < 5; i++)
  {
    Serial.print(analogRead(sensors[i]));
    Serial.print("   ");
  }
  Serial.println();
    // Extreme left turn when extremeLeft sensor detects dark region while extremeRight sensor detects white region
    if (analogRead(extremeLeft) > threshold[0] && analogRead(extremeRight) < threshold[4] )
    {
      Serial.println("left");
      lsp = 0; rsp = lfspeed;
      leftMotor.drive(0);
      rightMotor.drive(255);
      SerialBT.println("left");
      //left(leftMotor, rightMotor, lfspeed);
    }

    // Extreme right turn when extremeRight sensor detects dark region while extremeLeft sensor detects white region
    else if (analogRead(extremeRight) > threshold[4] && analogRead(extremeLeft) < threshold[0])
    { 
      Serial.println("right");
      lsp = lfspeed; rsp = 0;
      leftMotor.drive(255);
      rightMotor.drive(0);
      SerialBT.println("right");
      //right(leftMotor, rightMotor, lfspeed);
    }
    else if (analogRead(center) > threshold[2])
    {
      // arbitrary PID constans will be tuned later
      //forward(leftMotor, rightMotor);
      Kp = 0.2;
      Kd = 0.3;
      Ki = 0.01;
      Serial.print("center\t");
      Serial.println(Kp);
      carPID.setConstants(Kp, Ki, Kd);
      sp = (analogRead(centerRight) - analogRead(centerLeft)*2);
      Serial.print("error\t");
      Serial.println(sp);
      carPID.setSetpoint(sp);
      carPID.linefollow(leftMotor, rightMotor, lsp, rsp);
    }
  }
}
