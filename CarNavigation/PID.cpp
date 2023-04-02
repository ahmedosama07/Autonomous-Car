#include "PID.h"
PID::PID(){};
double PID::set_constants(double kP, double kI, double kD){
  kp = kP;
  ki = kI;
  kd = kD;
}
double PID::set_setPoint(double setPoint){
  setpoint = setPoint;
}
double PID::calc(double input){
    MV = input;
    diff = MV-prevMV;
    time1=millis();
    dt = time1 - last_time;
    error=(setpoint-MV);
    acc+=error*dt;
    acc=constrain(acc,lower,upper);
    PV=(error*kp)+(acc*ki)-((diff*kd)/dt);
    prevMV=MV;
    last_time = millis();
 
  return PV;
}
