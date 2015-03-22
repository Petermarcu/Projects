/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

const int pressureSensor = A0;
const int lightSensor = A1;

const int W1_9 = 3;
const int W1_10 = 4;
const int W1_11 = 5;
const int W1_12 = 6;
const int W1_13 = 7;

const int W2_9 = 9;
const int W2_10 = 10;
const int W2_11 = 11;
const int W2_12 = 12;
const int W2_13 = 13;

int sensorReading = 0; 

RF24 radio1(W1_9, W1_10);
RF24 radio2(W2_9, W2_10);

byte message[4] = {1,2,3,4};
byte data[4] = {0,0,0,0};

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  Serial.begin(9600);

Serial.println("Configure Radio 1");  
   radio1.begin();
   radio1.setChannel(2);
   radio1.setPayloadSize(4);
   radio1.setAutoAck(false);
   radio1.setCRCLength(RF24_CRC_8);

Serial.println("Configure Radio 2");
   
   radio2.setChannel(2);
   radio2.setPayloadSize(4);
   radio2.setAutoAck(false);
   radio2.setCRCLength(RF24_CRC_8);
   
Serial.println("Open Read");
   
   radio2.openReadingPipe(1,0xE7E7E7E7E7LL);
   radio2.startListening();

Serial.println("Open Write");

   radio1.openWritingPipe(0xE7E7E7E7E7LL);
  
}

// the loop function runs over and over again forever
void loop() {
  
  //sensorReading = analogRead(pressureSensor);
  //Serial.print("Pressure: ");
  //Serial.println(sensorReading);
  
  //sensorReading = analogRead(lightSensor);
  //Serial.print("Light: ");
  //Serial.println(sensorReading);

Serial.println("Sending Message");
  radio1.write(message, sizeof(message));

Serial.println("Reading");
  if(radio2.available())
  {  
     radio2.read(&data,sizeof(data));
   
     Serial.println(data[0]);
  }
  
  
  delay(100);
  
  
}
