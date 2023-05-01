#include "PID.h"
PID::PID(float kP, float kI, float kD)
{
  kp = kP;
  ki = kI;
  kd = kD;
}

void PID::setSpeeds(int v)
{
  lfspeed = v;
}

void PID::setConstants(float kP, float kI, float kD)
{
  kp = kP;
  ki = kI;
  kd = kD;
}

void PID::setSetpoint(int setPoint)
{
  sp = setPoint;
}

void PID::setConstrains(int lower_bound, int upper_bound)
{
  lower = lower_bound;
  upper = upper_bound;
}

int PID::calculate(int mv)
{

  P = mv;
  I = I + mv;
  D = mv - previousError;

  PIDvalue = (kp * P) + (ki * I) + (kd * D);
 
  return PIDvalue;
}

void PID::linefollow(Motor leftMotor, Motor rightMotor, int lsp, int rsp)
{
  error = sp;

  PIDvalue = calculate(error);
  previousError = error;

  lsp = lfspeed - PIDvalue;
  rsp = lfspeed + PIDvalue;

  if (lsp > 255) {
    lsp = 255;
  }
  if (lsp < 0) {
    lsp = 0;
  }
  if (rsp > 255) {
    rsp = 255;
  }
  if (rsp < 0) {
    rsp = 0;
  }
  leftMotor.drive(lsp);
  rightMotor.drive(rsp);
}


void PID::calibrate(Motor leftMotor, Motor rightMotor, int minValues[], int maxValues[], int threshold[])
{
  for ( int i = 1; i < 6; i++)
  {
    minValues[i] = analogRead(i);
    maxValues[i] = analogRead(i);
  }
  
  for (int i = 0; i < 3000; i++)
  {
    leftMotor.drive(50);
    rightMotor.drive(-50);

    for ( int i = 1; i < 6; i++)
    {
      if (analogRead(i) < minValues[i])
      {
        minValues[i] = analogRead(i);     //minValues for white regions
      }
      if (analogRead(i) > maxValues[i])
      {
        maxValues[i] = analogRead(i);     //maxValues for dark regions
      }
    }
  }

  for ( int i = 1; i < 6; i++)
  {
    threshold[i] = (minValues[i] + maxValues[i]) / 2;
    Serial.print(threshold[i]);
    Serial.print("   ");
  }
  Serial.println();
  
  leftMotor.drive(0);
  rightMotor.drive(0);
}
