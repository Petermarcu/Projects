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
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

const int pressureSensor = A0;
const int lightSensor = A1;

byte data[5];
int i = 0;
float famps = 1.724;

RF24 radio(9, 10);

void* message;

// the setup function runs once when you press reset or power the board
void setup() {
  
   Serial.begin(9600);   
   radio.begin(); 
   radio.setAutoAck(1);                    // Ensure autoACK is enabled 
   radio.enableAckPayload();               // Allow optional ack payloads 
   radio.setRetries(0,5);                 // Smallest time between retries, max no. of retries 
   radio.setPayloadSize(5);                // Here we are sending 5-byte payloads  
   radio.openWritingPipe(0xF0F0F0F0E1LL);        // Write to "Power" pipe 
   //radio.openReadingPipe(1,0xF0F0F0F0D2LL);      // Open a reading pipe on address 0, pipe 1  -- no need to read, ACK is fine 
   radio.startListening();                 // Start listening 
   radio.powerUp(); 
   radio.printDetails();                   // Dump the configuration of the rf unit for debugging 
}

// the loop function runs over and over again forever
void loop() {

   data[0]=1; 
   famps= famps + 7.7777; 
   memcpy(&data[1],&famps,4); 
   Serial.print("Data: ");
   Serial.println(famps);

  radioSend(data, 5);
  delay(1000);
  
}

void radioSend(const void *payload,int len) 
{
   byte gotByte; 
        
   //Write the payload 
   radio.stopListening();   
    
   if(radio.write(payload,len)) 
   { 
     if(!radio.available()) 
     {                              
       // If nothing in the buffer, we got an ack but it is blank 
       printf("Got blank response.\n");      
     } 
     else 
     {       
       while(radio.available() ) 
       {                       
           // If an ack with payload was received 
           radio.read(&gotByte,1);                  // Read it, and display the response time 
           printf("Got response %d\n\r",gotByte); 
       } 
     } 
   } 
   else 
   { 
     printf("Sending failed\n"); 
   } 
} 



