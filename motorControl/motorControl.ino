#include <Wire.h>
#include <DistanceSRF04.h>
#include <Adafruit_MotorShield.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "utility/Adafruit_PWMServoDriver.h"

#define CE_PIN   9  // Used when creating the radio object below...
#define CSN_PIN 10  // Used when creating the radio object below...

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotors[] = {AFMS.getMotor(1),AFMS.getMotor(2),AFMS.getMotor(3),AFMS.getMotor(4)};

const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

DistanceSRF04 Dist;
int distance;

int flashlight = 4;
int lightSensor = A2;

const float pi = 3.14;

int value[5];  

void setup(){
  Serial.begin(9600);
  Dist.begin(2,3);
  AFMS.begin();
  
  pinMode(flashlight, OUTPUT);
  
  radio.begin(); // Fire up the raido
  radio.openReadingPipe(1,pipe); // 'pipe' is the LongLong that was itialized above. 
  radio.startListening();
  
  
}
void loop(){
  
  boolean moving = false;
  
  
    
  
  
   if ( radio.available() )
  {
    // Read the data payload until we've received everything
    bool done = false;
    while (!done){
      
     analogRead(A1);
      int sensorValue = analogRead(lightSensor);
    Serial.println(sensorValue);

      done = radio.read( value, sizeof(value) );

     int xVal=0;
     int yVal=0;
     

      if(value[0]==166){
        moving=true;
        xVal=map(value[1],0,1023,-100,100)+1;
        yVal=map(value[2],0,1023,-100,100)-1;
      }
      
      else moving=false;  
    if(value[4]==0)
    calcMovement(xVal,yVal);
    else{
      distance = Dist.getDistanceInch();
      if(distance>12 || distance <0)forward();
      else{
        turnRight();
      }
    }
    delay(5); 
    
    
    boolean flashOn = sensorValue<=500;
    digitalWrite(flashlight, flashOn);
  
    }
    
    
  }
  else
  {   
      moving=false;
      stopMoving(); 
      Serial.println("no signal");
  }
    
 
  
  delay(500);
     
}

void forward(){
  for(int i =0; i<4; i++){  
     myMotors[i]->setSpeed(220);
     myMotors[i]->run(FORWARD);
   }
}
void backward(){
  for(int i =0; i<4; i++){  
     myMotors[i]->setSpeed(220);
     myMotors[i]->run(BACKWARD);
   }
}

void turnRight(){
  myMotors[2]->run(BACKWARD);
  myMotors[3]->run(FORWARD);
  for(int i =0; i<4; i++){  
     myMotors[i]->setSpeed(220);
   }
}

void turnLeft(){
  myMotors[3]->run(BACKWARD);
  myMotors[2]->run(FORWARD);
  for(int i =0; i<4; i++){  
     myMotors[i]->setSpeed(220);
   }
}

void stopMoving(){
  for(int i =0; i<4; i++){  
     myMotors[i]->setSpeed(0);
   }
}

void calcMovement(int xVal, int yVal){
  if(abs(xVal)>=10 || abs(yVal)>=10){
    double theta = atan2(yVal,xVal);
    if(theta<0)theta=pi+(pi+theta);
    Serial.print("Theta :");
    Serial.println(theta);
    double r=0;
    double l=0;
    
    if(value[3]>0){
      if(theta>=0 && theta<=(pi/2.0)){
        r = (100-(200*cos(theta)));
        l = (100);
        
        Serial.println("Right and Lefts");
        Serial.print(l);
        Serial.print(" , ");
        Serial.println(r);
        Serial.println();
      }
      
      else if(theta<=pi){
        l = (100+(200*cos(theta)));
        r = (100);
        
        Serial.println("Right and Lefts");
        Serial.print(l);
        Serial.print(" , ");
        Serial.println(r);
        Serial.println();
      }
      
      else if(theta<=(pi*1.5)){
        l = (100+(200*sin(theta)));
        r = (-100);
        
        Serial.println("Right and Lefts");
        Serial.print(l);
        Serial.print(" , ");
        Serial.println(r);
        Serial.println();
      }
      
      else if(theta<=(pi*2.0)){
        r = (100+(200*sin(theta)));
        l = (-100);
        
        Serial.println("Right and Lefts");
        Serial.print(l);
        Serial.print(" , ");
        Serial.println(r);
        Serial.println();
      }
      
      if(l<0)
        myMotors[3]->run(BACKWARD); //left
      else
        myMotors[3]->run(FORWARD);
      myMotors[3]->setSpeed(2*(abs(l)));
      
      if(r<0)
        myMotors[2]->run(BACKWARD); //right
      else
        myMotors[2]->run(FORWARD);
      myMotors[2]->setSpeed(2*(abs(r)));
      
    }
    
    
    //--------------
   else{
    if(theta>=0 && theta<=(pi/2.0)){
      r = (100-(100*cos(theta)));
      l = (100);
      
      Serial.println("Right and Lefts");
      Serial.print(l);
      Serial.print(" , ");
      Serial.println(r);
      Serial.println();
    }
    
    else if(theta<=pi){
      l = (100+(100*cos(theta)));
      r = (100);
      
      Serial.println("Right and Lefts");
      Serial.print(l);
      Serial.print(" , ");
      Serial.println(r);
      Serial.println();
    }
    
    else if(theta<=(pi*1.5)){
      l = (-100-(100*cos(theta)));
      r = (-100);
      
      Serial.println("Right and Lefts");
      Serial.print(l);
      Serial.print(" , ");
      Serial.println(r);
      Serial.println();
    }
    
    else if(theta<=(pi*2.0)){
      r = (100*sin(theta));
      l = (-100);
      
      Serial.println("Right and Lefts");
      Serial.print(l);
      Serial.print(" , ");
      Serial.println(r);
      Serial.println();
    }
    
    if(l<0)
      myMotors[3]->run(BACKWARD); //left
    else
      myMotors[3]->run(FORWARD);
    myMotors[3]->setSpeed(2*(abs(l)));
    
    if(r<0)
      myMotors[2]->run(BACKWARD); //right
    else
      myMotors[2]->run(FORWARD);
    myMotors[2]->setSpeed(2*(abs(r)));
    
  }
  }
  else{
    stopMoving();
    Serial.println("x and y are less than 10");
  }
//  delay(500);
}

  
