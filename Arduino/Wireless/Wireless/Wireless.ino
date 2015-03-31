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

int data[2] = {0,0};

int i = 0;

RF24 radio(9, 10);

void* message;

// the setup function runs once when you press reset or power the board
void setup() {
  
  Serial.begin(9600);

Serial.println("Configure Radio");  
   radio.begin();
   radio.setChannel(2);
   radio.setPayloadSize(8);
   radio.setAutoAck(false);
   //radio.setCRCLength(RF24_CRC_8);

Serial.println("Open Write");

   radio.openWritingPipe(0xE7E7E7E7E7LL);
}

// the loop function runs over and over again forever
void loop() {
  
//data[0] = analogRead(pressureSensor);
data[0]++;
Serial.print("Pressure: ");
Serial.println(data[0]);
  
//data[1] = analogRead(lightSensor);
data[1] = 200;
Serial.print("Light: ");
Serial.println(data[1]);


Serial.print("Sending Message: ");
  radio.write(data, sizeof(data));
 
  delay(100);  
}
