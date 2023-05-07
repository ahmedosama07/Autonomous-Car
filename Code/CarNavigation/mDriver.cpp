#include "mDriver.h"
#include <Arduino.h>

Motor::Motor(int In1pin, int In2pin, int PWMpin, int correction)
{
  In1 = In1pin;
  In2 = In2pin;
  PWM = PWMpin;
  Correction = correction;
  
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(PWM, OUTPUT);
}

void Motor::drive(int speed)
{
  speed = speed * Correction;
  if (speed>=0) fwd(speed);
  else rev(-speed);
}
void Motor::drive(int speed, int duration)
{
  drive(speed);
  delay(duration);
}

void Motor::fwd(int speed)
{
   digitalWrite(In1, HIGH);
   digitalWrite(In2, LOW);
   analogWrite(PWM, speed);

}

void Motor::rev(int speed)
{
   digitalWrite(In1, LOW);
   digitalWrite(In2, HIGH);
   analogWrite(PWM, speed);
}

void Motor::brake()
{
   digitalWrite(In1, HIGH);
   digitalWrite(In2, HIGH);
   analogWrite(PWM,0);
}

void Motor::stop()
{
   digitalWrite(In1, LOW);
   digitalWrite(In2, LOW);
   analogWrite(PWM,0);
}

void forward(Motor left, Motor right, int speed)
{
  left.drive(speed);
  right.drive(speed);
}
void forward(Motor left, Motor right)
{
  left.drive(DEFAULTSPEED);
  right.drive(DEFAULTSPEED);
}


void back(Motor left, Motor right, int speed)
{
  int temp = abs(speed);
  left.drive(-temp);
  right.drive(-temp);
}
void back(Motor left, Motor right)
{
  left.drive(-DEFAULTSPEED);
  right.drive(-DEFAULTSPEED);
}
void left(Motor left, Motor right, int speed)
{
  int temp = abs(speed)/2;
  left.drive(-temp);
  right.drive(temp);
  
}


void right(Motor left, Motor right, int speed)
{
  int temp = abs(speed)/2;
  left.drive(temp);
  right.drive(-temp);
  
}
void brake(Motor left, Motor right)
{
  left.brake();
  right.brake();
}

void stop(Motor left, Motor right)
{
  left.stop();
  right.stop();
}
