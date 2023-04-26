#include "PID.h"
PID::PID(double kP, double kI, double kD)
{
  kp = kP;
  ki = kI;
  kd = kD;
};

void PID::set_constants(double kP, double kI, double kD)
{
  acc=0;
  last_time=0;
  kp = kP;
  ki = kI;
  kd = kD;
}

void PID::set_setpoint(double setPoint)
{
  setpoint = setPoint;
}

void PID::set_constrains(double lower_bound, double upper_bound)
{
  lower = lower_bound;
  upper = upper_bound;
}

double PID::calc(double mv)
{
  if(!enabled) return 0;
  double err = setpoint - mv;
  double dt = (millis() - last_time)/1000.0;


  double diff=0;
  if(last_time)
  {
      diff = (mv-last_mv);
      
      diff/=dt;
      acc+=err*dt;
      acc=constrain(acc,lower,upper);

  }
  last_time = millis();
  last_mv = mv;
 
  return (err*kp)+(acc*ki)-(diff*kd);

}
