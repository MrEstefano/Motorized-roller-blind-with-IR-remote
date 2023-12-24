
/*
 Name:    step_drive.ino
 Author:  mertwhocodes
*/


#include <IRremote.h>
#include <IRremoteInt.h>


#define EN_PIN 10
#define SLEEP 5
const int stepPin = 4; 
const int dirPin = 3; 
const int ms1Pin = 11;
const int ms2Pin = 8;
const int ms3Pin = 7;

float waitTime = 0.003;
int accelerationTerm = 3000;
int roundedWaitTime;
unsigned long timer1;
unsigned long timer2;
unsigned long difference;


int RECV_PIN = 9;          //  The digital pin that the signal pin of the sensor is connected to
IRrecv receiver(RECV_PIN);  //  Create a new receiver object that would decode signals to key codes
decode_results results;  
int motorFlag = 1; 

float positiveAcceleration(float waitTime);
float negativeAcceleration(float waitTime);

void setup() {
  Serial.begin(9600);       //  Setup serial port to send key codes to computer
  pinMode(SLEEP,OUTPUT);  
  pinMode(EN_PIN,OUTPUT); 
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(ms1Pin,OUTPUT);
  pinMode(ms2Pin,OUTPUT);
  pinMode(ms3Pin,OUTPUT);
   
  digitalWrite(ms1Pin,LOW);
  digitalWrite(ms2Pin,HIGH);
  digitalWrite(ms3Pin,LOW);
   digitalWrite(EN_PIN,LOW);
  receiver.enableIRIn();    //  Enable receiver so that it would start processing infrared signals
  
}

void loop() {
  
  if (receiver.decode(&results)) {            //  Decode the button code and put it in "results" variable  
    if (results.value == 0x10EF609F && motorFlag == 0 ){          //  Key '1' is pressed which belongs to the blue LED
      motorFlag = 1;      
      digitalWrite(SLEEP,HIGH); 
      digitalWrite(dirPin,HIGH);
      for(int y = 0; y < 32700; y++) {                //accelerate for 3000 steps
        waitTime = positiveAcceleration(waitTime);
        roundedWaitTime = round(waitTime*1000000);
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(roundedWaitTime);
        digitalWrite(stepPin, LOW);
      }     
      for(int x = 0; x < 1984; x++) {                //deccelerate for 1984 steps
        waitTime = negativeAcceleration(waitTime);
        roundedWaitTime = round(waitTime*1000000);
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(roundedWaitTime);
        digitalWrite(stepPin, LOW);
        if(x==1983){
          digitalWrite(SLEEP,LOW);
        }
      } 
    }   
    if (results.value == 0x10EFE01F  && motorFlag == 1 ) {          //  Key '1' is pressed which belongs to the blue LED
      motorFlag = 0;
      digitalWrite(SLEEP,HIGH); 
      digitalWrite(dirPin,LOW);
      for(int z = 0; z < 32700; z++) {                //accelerate for 3000 steps
        waitTime = positiveAcceleration(waitTime);
        roundedWaitTime = round(waitTime*1000000);
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(roundedWaitTime);
        digitalWrite(stepPin, LOW);
      }     
      for(int w = 0; w < 1984; w++) {                //deccelerate for 1984 steps
        waitTime = negativeAcceleration(waitTime);
        roundedWaitTime = round(waitTime*1000000);
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(roundedWaitTime);
        digitalWrite(stepPin, LOW);
        if(w==1983){
          digitalWrite(SLEEP,LOW);
        }
      }   
    }                              //  Blue LED is off, turn it on and set update corresponding state variable
    receiver.resume();                        //  Continue listening for new signals
    delay(1000); 
  }
}


//function to return the next delay value for positive acceleration
float positiveAcceleration(float waitTime) {
    float dVelocity = waitTime * accelerationTerm;
    waitTime = 1/(dVelocity+1/waitTime);
    if (waitTime < 0.00025){
      waitTime = 0.00025;
    }
    return waitTime;
}

//function to return the next delay value for negative acceleration
float negativeAcceleration(float waitTime) {
    float dVelocity = waitTime * -1 * accelerationTerm;
    waitTime = 1/(dVelocity+1/waitTime);
    if (waitTime > 0.003){
      waitTime = 0.003;
    }
    return waitTime;
}
