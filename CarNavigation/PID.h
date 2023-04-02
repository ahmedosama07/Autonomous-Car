#include <Arduino.h>
static int last_time=0;
class PID{
  public:
    double kp=0,ki=0,kd=0;
    double error=0;
    double acc=0;
    double prevMV=0;
    double time1=0;
    double diff=0;
    double PV=0;
    double setpoint=0;
    double MV=0;
    int lower=-255, upper=255;
    double dt;
    PID();
    double calc(double input);
    double set_constants(double kP, double kI, double kD);
    double set_setPoint(double setPoint);
};
