/*************************************************** 
  This is an example for the Adafruit NEC Remote Control

  Designed specifically to work with the Adafruit NEC Remote Control
	----> http://www.adafruit.com/products/389
  and IR Receiver Sensor
	----> http://www.adafruit.com/products/157

  These devices use IR to communicate, 1 pin is required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
 

#include "Adafruit_NECremote.h"
 
// Connect a 38KHz remote control sensor to the pin below

#define RELAY1  A0  
#define LAMP   1
#define IRpin   2
#define ButtonPin 3
#define LightSensor A5

const int RED_LED_PIN = 11;    //Red LED Pin
const int GREEN_LED_PIN = 10; //Green LED Pin
const int BLUE_LED_PIN = 9;  //Blue LED Pin
 
Adafruit_NECremote remote(IRpin);
boolean lampOn = false;
boolean runColors = false;

// Used to store the current intensity level of the individual LEDs
int redIntensity = 0;
int greenIntensity = 0;
int blueIntensity = 0;

int threshhold = 350;

// Length of time we spend showing each color
const int DISPLAY_TIME = 80; // In milliseconds

void setup(void) {
  
  Serial.begin(115200);
  Serial.println("Ready to decode IR!");
  pinMode(RELAY1, OUTPUT);
  pinMode(LightSensor,INPUT);

  digitalWrite(RELAY1,HIGH); 
  
  pinMode(ButtonPin, INPUT);
  attachInterrupt(1,pressed,LOW);
}

int lastcode = -1;

void pressed(){
  //Serial.print("OH SHIT!");
  //if(digitalRead(ButtonPin) == LOW && lampOn ==false){
  if(lampOn ==false){
    digitalWrite(RELAY1,LOW);           // Turns ON Relays 1
    lampOn = true;
  }  
    
  //else if(digitalRead(ButtonPin) == LOW && lampOn ==true){
  else if(lampOn==true){
    digitalWrite(RELAY1,HIGH);           // Turns ON Relays 1
    lampOn = false;
  }      
}

void loop(void) {
  Serial.print(analogRead(LightSensor));
  if(analogRead(LightSensor)<threshhold){
    runColors=true;
  }
  else{
    runColors=false;
    digitalWrite(BLUE_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
  }
  
  if(runColors==false){
  // You can set the listen() time out to 'n' seconds
  int c = remote.listen(5);  // seconds to wait before timing out!
  // Or you can wait 'forever' for a valid code
  //int c = remote.listen();  // Without a #, it means wait forever

  if (c >= 0) {
    Serial.print("Received code #"); 
    Serial.println(c, DEC);
    lastcode = c;
    if(c == LAMP && lampOn == false){
     digitalWrite(RELAY1,LOW);           // Turns ON Relays 1
     lampOn = true;
    }
    else if(c ==LAMP && lampOn == true){
     digitalWrite(RELAY1,HIGH);
     lampOn = false;
    }
  } else if (c == -3) {
    Serial.print("Repeat (");
    Serial.print(lastcode);
    Serial.println(")");
  } else if (c == -2) {
    Serial.println("Data error");
  } else {
    Serial.println("Timed out waiting!");
  }
  

  }
  
  while(runColors){
// Cycle color from red through to green
  // (In this loop we move from 100% red, 0% green to 0% red, 100% green)
  greenIntensity =0;
  redIntensity = 255;
  
  analogWrite(GREEN_LED_PIN, greenIntensity);
  analogWrite(RED_LED_PIN, redIntensity);
  delay(2000);
  
  for (greenIntensity = 0; greenIntensity <= 255; greenIntensity+=5) {
        redIntensity = 255-greenIntensity;
       
        analogWrite(GREEN_LED_PIN, greenIntensity);
        analogWrite(RED_LED_PIN, redIntensity);
      
        delay(DISPLAY_TIME);

  }
    if(analogRead(LightSensor)>threshhold){runColors==false; break;}

  // Cycle color from green through to blue
  // (In this loop we move from 100% green, 0% blue to 0% green, 100% blue)  
  for (blueIntensity = 0; blueIntensity <= 255; blueIntensity+=5) {
        greenIntensity = 255-blueIntensity;
        analogWrite(BLUE_LED_PIN, blueIntensity);
        analogWrite(GREEN_LED_PIN, greenIntensity);
       
        delay(DISPLAY_TIME);
  }
   if(analogRead(LightSensor)>threshhold){runColors==false; break;}

  // Cycle cycle from blue through to red
  // (In this loop we move from 100% blue, 0% red to 0% blue, 100% red)    
  for (redIntensity = 0; redIntensity <= 255; redIntensity+=5) {
        blueIntensity = 255-redIntensity;
        analogWrite(RED_LED_PIN, redIntensity);
        analogWrite(BLUE_LED_PIN, blueIntensity);
     
        delay(DISPLAY_TIME);
  }
     if(analogRead(LightSensor)>threshhold){runColors==false; break;}
 }
  
}
