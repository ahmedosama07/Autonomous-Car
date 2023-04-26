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
int R_motor = 180;  //speed of right motor
// Second motor controler
int MC_2_R = 23;
int MC_2_l = 25;
int l_motor = 180;  //speed of left motor

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
}

void loop()
{
  val_1 = digitalread(IR_1); // read the digital value from the first sensor
  val_2 = digitalread(IR_2); // read the digital value from the second sensor
  val_3 = digitalread(IR_3); // read the digital value from the third sensor
  val_4 = digitalread(IR_4); // read the digital value from the fourth sensor
  val_5 = digitalread(IR_5); // read the digital value from the fifth sensor

  // if conditions for the movement of the car
  if (val_1 == 1)
    &&(val_2 == 1) && (val_3 == 1) && (val_4 == 1) && (val_5 == 1) // condition that the line is straight for the robot
    {
      forward()
      
    }
  else if ((val_5) == 0)
  { // condition that the line is left for the robot
     left()
  }
  else
    ((val_4) == 0)
    {
       right() // condition that the line is right for the robot
    }

    void forward()
    {
      digitalRead(MC_1_R, LOW);
      digitalRead(MC_1_l, LOW);
      digitalRead(MC_2_R, LOW);
      digitalRead(MC_2_l, LOW);
      analogWrite(MC_1_R,R_motor);
      analogWrite(MC_2_l,l_motor);
      digitalRead(MC_1_R, HIGH);
      digitalRead(MC_2_l, HIGH);
    }

  void left()
  {
    digitalRead(MC_1_R, LOW);
    digitalRead(MC_1_l, LOW);
    digitalRead(MC_2_R, LOW);
    digitalRead(MC_2_l, LOW);
    analogWrite(MC_2_l,l_motor);
    digitalRead(MC_2_l, HIGH);
    delay(1000);
  }

  void right()
  {
    digitalRead(MC_1_R, LOW);
    digitalRead(MC_1_l, LOW);
    digitalRead(MC_2_R, LOW);
    digitalRead(MC_2_l, LOW);
    analogWrite(MC_2_R,R_motor);
    digitalRead(MC_1_R, HIGH);
    delay(1000);
  }
}
