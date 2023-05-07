#ifndef PID_h
#define PID_h
#include "mDriver.h"
#include <Arduino.h>


class PID
{
  public:
    PID(float kP, float kI, float kD);
    int calculate(int mv);
    void setConstants(float kP, float kI, float kD);
    void setSetpoint(int setPoint);
    void setSpeeds(int v);
    void setConstrains(int lower_bound, int upper_bound);
    void calibrate(Motor leftMotor, Motor rightMotor, int minValues[], int maxValues[], int threshold[], int sensors[]);
    void linefollow(Motor leftMotor, Motor rightMotor, int lsp, int rsp);
  private:
  float kp=0,ki=0,kd=0;
    int sp = 0;
    int error = 0;
    int lfspeed = 0;
    int lower = -255, upper = 255;
    int P=0, D=0, I=0, previousError=0, PIDvalue=0;
};
#endif
