-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 //@project: line follower robot using esp32,IR sensors,DC motors,PCP
 //@authors:Ahmed Abdelhakem,Ahmed Osama,Mostafa Mohamed,Mazen Mohamed,Mohamed Ashraf
 //@date :24/4/2023
 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

 // the IR sensors of the robot
const int IR_1 = 36; // pin of first IR sensor
const int IR_2 = 39;                                                // pin of second IR sensor
const int IR_3 = 34;                                               // pin of third IR sensor
const int IR_4 = 35;                                              // pin of fourth IR sensor **this sensor should be at the right of the body
const int IR_5 = 33;                                              // pin of fifth IR sensor **this sensor should be at the right of the body

// the values measured by IR sensors
float val_1 = 0; // value measured by first IR sensor
float val_2 = 0; // value measured by second IR sensor
float val_3 = 0; // value measured by third IR sensor
float val_4 = 0; // value measured by fourth IR sensor
float val_5 = 0; // value measured by fifth IR sensor

// Right motor controler
int MC_1_R = 21;
int MC_1_l = 22;
int EN1=13;
int R_motorspeed = 180;  //speed of right motor
// Second motor controler
int MC_2_R = 23;
int MC_2_l = 25;
int EN2=19;
int l_motorspeed = 180;  //speed of left motor

void setup()
{
  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);
  pinMode(IR_3, INPUT);
  pinMode(IR_4, INPUT);
  pinMode(IR_5, INPUT);
  pinMode(MC_1_R, OUTPUT);
  pinMode(MC_1_l, OUTPUT);
  pinMode(MC_2_R, OUTPUT);
  pinMode(MC_2_l, OUTPUT);
  pinMode(EN1,OUTPUT);
  pinMode(EN2,OUTPUT);

}

void loop()
{
  val_1 = digitalRead(IR_1); // read the digital value from the first sensor
  val_2 = digitalRead(IR_2); // read the digital value from the second sensor
  val_3 = digitalRead(IR_3); // read the digital value from the third sensor
  val_4 = digitalRead(IR_4); // read the digital value from the fourth sensor
  val_5 = digitalRead(IR_5); // read the digital value from the fifth sensor

  // if conditions for the movement of the car
  if (val_1 == 1)
    &&(val_2 == 1) && (val_3 == 1) && (val_4 == 1) && (val_5 == 1) // condition that the line is straight for the robot
    {
      forward()
      
    }
  else if ((val_5) == 0)&&(((val_4) == 1)
  {                                // condition that the line is left for the robot
     left()
  }
  else
    ((val_4) == 0) && ((val_5) == 1)
    {
       right()                        // condition that the line is right for the robot
    }

    void forward()
    {

      analogWrite(EN1,R_motorspeed);
      analogWrite(EN2,l_motorspeed);
      digitalWrite(MC_1_R, HIGH);
      digitalWrite(MC_1_l, LOW);
      digitalWrite(MC_2_R, LOW);
      digitalWrite(MC_2_l, HIGH);
    }

  void left()
  {
    analogWrite(EN1,R_motorspeed);
    analogWrite(EN2,220);
    digitalWrite(MC_2_R, LOW);
    digitalWrite(MC_2_l, HIGH);
    digitalWrite(MC_1_R, HIGH);
    digitalWrite(MC_1_l, LOW);
    
  }

  void right()
  {
    analogWrite(EN1,220);
     analogWrite(EN2,l_motorspeed);
    digitalWrite(MC_1_R, HIGH);
    digitalWrite(MC_1_l, LOW);
     digitalWrite(MC_2_R, LOW);
    digitalWrite(MC_2_l, HIGH);
  }
}