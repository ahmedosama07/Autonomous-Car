#include "PID.h"
PID pid_test;
double rec = 0;
double outPut = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pid_test.set_constants(0,0.1,0);
  pid_test.set_setPoint(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  outPut = pid_test.calc(rec);
  Serial.println(outPut);
  rec++;
}
