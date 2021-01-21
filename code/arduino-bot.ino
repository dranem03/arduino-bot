#include <Servo.h>
#include <AFMotor.h>
#define servo1 9 //5  11
#define servo2 6 //2  6
#define servo3 3 //3   9
#define servo4 5 //4  10
/*
AF_DCMotor motor1(1, MOTOR34_1KHZ); 
AF_DCMotor motor4(2, MOTOR12_1KHZ); 
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor2(4, MOTOR34_1KHZ);
*/
Servo mservo1, mservo2, mservo3, mservo4;

int pos = 0;
char Incoming_value = 0;
char usechar = "0";
bool carmode = false;
int myspeed = 255;

const int E1 = 10;
const int M1 = 12;
const int E2 =11;
const int M2 = 13;
const int BUZZER = 4;

unsigned long currentMill = 0;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  mservo1.attach(servo1); // 150-50
  mservo2.attach(servo2); // 0-180
  mservo3.attach(servo3); // 40-120
  mservo4.attach(servo4); // 0-35
  // beep sound after boot
  digitalWrite(BUZZER, HIGH);
  delay(50);
  digitalWrite(BUZZER, LOW);
  delay(90);
  digitalWrite(BUZZER, HIGH);
  delay(150);
  digitalWrite(BUZZER, LOW);
  delay(90);
  digitalWrite(BUZZER, HIGH);
  delay(50);
  digitalWrite(BUZZER, LOW);
  delay(50);
  
  //default servo/arm position
  mservo1.write(0); //up-down
  mservo2.write(90); //left-right
  mservo3.write(80); //forward-back
  mservo4.write(0); //clamp
}

int srv1 = 0;
int srv2 = 90;
int srv3 = 80;
int srv4 = 0;

void loop() 
{

  if(Serial.available() > 0)  
  {
    Incoming_value = Serial.read();
    Serial.println(Incoming_value);
    usechar = Incoming_value;
  }

  if(usechar=='E'){
    if(!carmode){
      carmode = true;
      //Serial.println("Carmode On");
      digitalWrite(BUZZER, HIGH);
      delay(150);
      digitalWrite(BUZZER, LOW);
      delay(150);
      digitalWrite(BUZZER, HIGH);
      delay(150);
      digitalWrite(BUZZER, LOW);
      delay(150);
    }
  }
  if(usechar=='H'){
    if(carmode){
      carmode = false;
      digitalWrite(BUZZER, HIGH);
      delay(50);
      digitalWrite(BUZZER, LOW);
      delay(50);
      digitalWrite(BUZZER, HIGH);
      delay(50);
      digitalWrite(BUZZER, LOW);
      //Serial.println("Carmode Off");
    }
    else{
      srv1 = 0;
      srv2 = 90;
      srv3 = 80;
      srv4 = 0;
      mservo1.write(srv1); //up-down
      mservo2.write(srv2); //left-right
      mservo3.write(srv3); //forward-back
      mservo4.write(srv4); //clamp
    }
    delay(150);
  }

  if(usechar=='o'){
    if(srv4<45){
      for(int i=srv4; i<=45; i++){
        srv4 = i;
        mservo4.write(i);
        delay(5);
      }
    }
  }
  if(usechar=='c'){
    if(srv4>0){
      for(int i=srv4; i>0; i--){
        srv4 = i;
        mservo4.write(i);
        delay(5);
      }
    }
  }
  
  if(usechar=='0')
  {
    Stop();
  }
  
  if(usechar=='F')
  {
    if(srv1<80)
    {
      srv1++;
      mservo1.write(srv1);
      //Serial.println("F: " + String(srv1));
      delay(10);
     }
  }
  
  if(usechar=='I')
  {
    if(srv1>0)
    {
      srv1--;
      mservo1.write(srv1);
      
      delay(10);
     }
  }

  if(usechar=='C')
  {
    if(carmode){
      left();
    }
    else{
      if(srv2<=180)
      {
        srv2++;
        mservo2.write(srv2);
        
        delay(10);
       }
    }
  }

  if(usechar=='D')
  {
    if(carmode){
      right();
    }
    else{
      if(srv2>0)
      {
        srv2--;
        mservo2.write(srv2);
        
        delay(10);
       }
    }
  }

  if(usechar=='G')
  {
    if(carmode){
      forward();
    }
    else{
      if(srv3<140)
      {
        srv3++;
        mservo3.write(srv3);
        //Serial.println("position: " + String(srv3));
        delay(10);
       }
    }
   }
   
   if(usechar=='J')
   {
    if(carmode){
      backward();
    }
    else{
      if(srv3>50)
      {
        srv3--;
        mservo3.write(srv3);
        //Serial.println("position: " + String(srv3));
        delay(10);
       }
    }
   }

   //-----

   if(usechar=='A')
  {
    if(srv4<45)
    {
      srv4++;
      mservo4.write(srv4);
      
      delay(5);
     }
   }
   
   if(usechar=='B')
   {
    if(srv4>0)
    {
      srv4--;
      mservo4.write(srv4);
      
      delay(5);
     }
   }

   
      
}
void forward(){
  digitalWrite(M1, HIGH);
  analogWrite(E1, 255);
  digitalWrite(M2, HIGH);
  analogWrite(E2, 255);
}

void left(){
  digitalWrite(M1, HIGH);
  analogWrite(E1, 255);
  digitalWrite(M2, LOW);
  analogWrite(E2, 255);
}

void right(){
  digitalWrite(M1, LOW);
  analogWrite(E1, 255);
  digitalWrite(M2, HIGH);
  analogWrite(E2, 255);
}

void backward(){
  if(millis() > (currentMill+500)){
    currentMill = millis();
    digitalWrite(BUZZER, !digitalRead(BUZZER));
  }
  analogWrite(E1, 255);
  digitalWrite(M1, LOW);
  analogWrite(E2, 255);
  digitalWrite(M2, LOW);
  
}

void Stop(){
  analogWrite(E1, 0);
  digitalWrite(M1, 0);
  analogWrite(E2, 0);
  digitalWrite(M2, 0);
  digitalWrite(BUZZER, LOW);
}
/*
void forward()
{
  motor1.setSpeed(myspeed); //Define maximum velocity
  motor1.run(FORWARD); //rotate the motor clockwise
  motor2.setSpeed(myspeed); //Define maximum velocity
  motor2.run(FORWARD); //rotate the motor clockwise
  motor3.setSpeed(myspeed);//Define maximum velocity
  motor3.run(FORWARD); //rotate the motor clockwise
  motor4.setSpeed(myspeed);//Define maximum velocity
  motor4.run(FORWARD); //rotate the motor clockwise
}

void back()
{
  motor1.setSpeed(myspeed); //Define maximum velocity
  motor1.run(BACKWARD); //rotate the motor anti-clockwise
  motor2.setSpeed(myspeed); //Define maximum velocity
  motor2.run(BACKWARD); //rotate the motor anti-clockwise
  motor3.setSpeed(myspeed); //Define maximum velocity
  motor3.run(BACKWARD); //rotate the motor anti-clockwise
  motor4.setSpeed(myspeed); //Define maximum velocity
  motor4.run(BACKWARD); //rotate the motor anti-clockwise
}

void left()
{
  motor1.setSpeed(255); //Define maximum velocity
  motor1.run(BACKWARD); //rotate the motor anti-clockwise
  motor2.setSpeed(255); //Define maximum velocity
  motor2.run(FORWARD); //rotate the motor anti-clockwise
  motor3.setSpeed(255); //Define maximum velocity
  motor3.run(FORWARD);  //rotate the motor clockwise
  motor4.setSpeed(255); //Define maximum velocity
  motor4.run(BACKWARD);  //rotate the motor clockwise
}

void right()
{
  motor1.setSpeed(255); //Define maximum velocity
  motor1.run(FORWARD); //rotate the motor clockwise
  motor2.setSpeed(255); //Define maximum velocity
  motor2.run(BACKWARD); //rotate the motor clockwise
  motor3.setSpeed(255); //Define maximum velocity
  motor3.run(BACKWARD); //rotate the motor anti-clockwise
  motor4.setSpeed(255); //Define maximum velocity
  motor4.run(FORWARD); //rotate the motor anti-clockwise
} 

void Stop()
{
  motor1.setSpeed(0); //Define minimum velocity
  motor1.run(RELEASE); //stop the motor when release the button
  motor2.setSpeed(0); //Define minimum velocity
  motor2.run(RELEASE); //rotate the motor clockwise
  motor3.setSpeed(0); //Define minimum velocity
  motor3.run(RELEASE); //stop the motor when release the button
  motor4.setSpeed(0); //Define minimum velocity
  motor4.run(RELEASE); //stop the motor when release the button
}
*/
