#include <stdio.h>
#include <string.h>
#include <time.h>
#include "RF24/RF24.h"
#include <sys/resource.h>

// Setup for GPIO 15 CE and CE0 CSN with SPI Speed @ 8Mhz
RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);

// First pipe is for writing, 2nd, 3rd, 4th, 5th & 6th is for reading...
const uint64_t pipes[1] = { 0xE7E7E7E7 };

// Payload size - 32 is the default.  Must be the same on transmitter
const uint8_t MAX_PAYLOAD_SIZE = 8;
uint8_t pipeNo;
uint16_t bytesRecv[2] = {0,0};

time_t rawtime;
struct tm * timeinfo;
struct rusage memInfo;

int main()
{
    printf("Starting");

    //tag log files
    fprintf(stderr,"-\n-\n-\nHubMaster Start - %s\n-\n",ctime(&rawtime));
    fprintf(stdout,"-\n-\n-\nHubMaster Start - %s\n-\n",ctime(&rawtime));

    //setup radio
    radio.begin();
    radio.setAutoAck(0);                    // Ensure autoACK is enabled
    radio.setRetries(15,15);                // Smallest time between retries, max no. of retries
    radio.setPayloadSize(MAX_PAYLOAD_SIZE);                // Here we are sending 1-byte payloads to test the call-response speed
    radio.printDetails();                   // Dump the configuration of the rf unit for debugging
    radio.setChannel(2);

    // Open 6 pipes for readings ( 5 plus pipe0, also can be used for reading )
    radio.openReadingPipe(1, 0xE7E7E7E7E7);

    //Start listening!
    radio.startListening();

    //Loop, checking for radio packets
    while(1)
    {
      //Check if something's on the radio
      if(radio.available())
      {
        //read data from the pipe
        //radio.flush_tx();
        radio.read(&bytesRecv,sizeof(bytesRecv));
        printf("Sensor #1: %d / Sensor #2: %d)\n",bytesRecv[0],bytesRecv[1]);
      }

      delay(100);
    }
    return 0;
}
