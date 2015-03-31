#include <stdio.h>
#include <string.h>
#include <openssl/hmac.h>
#include <openssl/pem.h>
#include <time.h>
#include <curl/curl.h>
#include <RF24/RF24.h>
#include <sys/resource.h> 

void readDeviceData();
void callWebAPI(char *urlRaw,char *authHeader);
void createAuthHeader (char *key,char *keyName,char *url,char *authHeader);
char * base64encode (const void *b64_encode_me, int encode_this_many_bytes,int *resLen);
int createAndPostData(char *authHeader);
void timeStamp(FILE *file);
void flushFileHandles();

// Setup for GPIO 15 CE and CE0 CSN with SPI Speed @ 8Mhz
RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);

// First pipe is for writing, 2nd, 3rd, 4th, 5th & 6th is for reading...
const uint64_t pipes[6] = { 0xF0F0F0F0D2LL, 0xF0F0F0F0E1LL, 
                            0xF0F0F0F0E2LL, 0xF0F0F0F0E3LL, 
                            0xF0F0F0F0F1,   0xF0F0F0F0F2 };

// Payload size - 32 is the default.  Must be the same on transmitter
const uint8_t MAX_PAYLOAD_SIZE = 32;  
const uint8_t MAX_POSTDATA_SIZE = 128+1; 
uint8_t pipeNo;
int lastPayloadLen;
uint8_t bytesRecv[MAX_PAYLOAD_SIZE+1];
char postData[MAX_POSTDATA_SIZE];

char *url=(char*)"https://homeautomation-ns.servicebus.windows.net/homeautomation/messages?api-version=2014-01";

FILE *logFile;
time_t rawtime;
struct tm * timeinfo;
struct rusage memInfo;

int main()
{
    //open file for logging
    logFile = fopen("/home/pi/code/rfHub/HubMasterLog.txt", "w");
    freopen ("/home/pi/code/rfHub/HubMasterStderr.txt","w",stderr);
    freopen ("/home/pi/code/rfHub/HubMasterStdout.txt","w",stdout);

    //tag log files
    time(&rawtime);
    fprintf(logFile,"-\n-\n-\nHubMaster Start - %s-\n\n",ctime(&rawtime));
    fprintf(stderr,"-\n-\n-\nHubMaster Start - %s\n-\n",ctime(&rawtime));
    fprintf(stdout,"-\n-\n-\nHubMaster Start - %s\n-\n",ctime(&rawtime));

    //setup radio
    radio.begin();
    radio.setAutoAck(1);                    // Ensure autoACK is enabled
    radio.enableAckPayload();               // Allow optional ack payloads
    radio.setRetries(15,15);                // Smallest time between retries, max no. of retries
    //radio.setPayloadSize(5);                // Here we are sending 1-byte payloads to test the call-response speed
    radio.printDetails();                   // Dump the configuration of the rf unit for debugging
    radio.enableDynamicPayloads();          // Read size off chip

    // Open 6 pipes for readings ( 5 plus pipe0, also can be used for reading )
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]); //Power reading pipe
    radio.openReadingPipe(2,pipes[2]); //Motion sensor pipe
    radio.openReadingPipe(3,pipes[3]); //Temperature sensors pipe
    radio.openReadingPipe(4,pipes[4]);
    radio.openReadingPipe(5,pipes[5]);

    //Start listening!
    radio.startListening();

    //Loop, checking for radio packets
    while(1)
    {
      //Check if something's on the radio
      if(radio.available(&pipeNo))
      {  
        //timeStamp(logFile);

        //read data from the pipe
        readDeviceData();

        //Create new SAS authorization header
        char *baseUrl=(char *)"homeautomation-ns.servicebus.windows.net";
        char *authHeader=(char*)malloc(256);
        createAuthHeader((char*)"3RdEfuPG4TeMbsBRhLgaCnyoq5ZttZpJWFdxajN0rZM=",(char*)"saspolicy",baseUrl,authHeader);

        //Send to Azure
        createAndPostData(authHeader);

        //Free memory
        free(authHeader);
        flushFileHandles();
      }

      //check for carrier
      delay(150);
      if (radio.testCarrier())
      {
        fprintf(logFile,"^");
        flushFileHandles();
      }
      delay(350);
    }
    return 0;
}

void timeStamp(FILE *file)
{
    //Dump header
    getrusage(RUSAGE_SELF,&memInfo);
    time(&rawtime);
    timeinfo = localtime (&rawtime);
    char *time=asctime(timeinfo);
    time[(strlen(time))-1]=0;
    fprintf(file,"Mem:%lu %s: ",memInfo.ru_maxrss,time);
    fflush(file);  
}

void flushFileHandles()
{
    static int newLineCounter = 0;

    //If time for a newline, do it now
    if(++newLineCounter > 80)
    {
       timeStamp(logFile);
       fprintf(logFile,"\n");
       newLineCounter=0;
    }

    fflush(logFile);  
    fflush(stderr); 
    fflush(stdout); 
}

int createAndPostData(char *authHeader)
{
    time_t seconds_past_epoch = time(0);
    int retVal=0;

    //Init postData
    for(int i=0;i<MAX_POSTDATA_SIZE;i++)
      postData[i]=0;

    //Power
    if(pipeNo==1)
    {
       uint8_t addr;
       float reading;

	addr=bytesRecv[0];
       memcpy(&reading,&bytesRecv[1],4);
       //fprintf(logFile,"Addr: %d  Reading: %f\n",addr,reading);
       sprintf(postData,"{'DeviceName':'Power%d','DeviceDate':'%ld','DeviceData1':'%f'}",addr,seconds_past_epoch,reading);

       fprintf(logFile,".");  //Small indication for log file
       fflush(logFile);
    }

    //Motion
    if(pipeNo==2)
    {
      sprintf(postData,"{'DeviceName':'%s','DeviceDate':'%ld','DeviceData1':'0'}","Motion",seconds_past_epoch);

       fprintf(logFile,"|");  //Small indication for log file
       fflush(logFile);
    }

    //Thermometer
    if(pipeNo==3)
    {
       static uint8_t lastDoorOpenFlag=-1;
       bool openStateChangedFlag=false;
       uint8_t thermNum;
       float reading;
       uint8_t doorOpenFlag=-1; //0 is closed, 1 is open for door

       thermNum=bytesRecv[0];
       memcpy(&reading,&bytesRecv[1],4);
       if(lastPayloadLen>5)
       {
          doorOpenFlag=bytesRecv[5];

          openStateChangedFlag=false;
          if(doorOpenFlag!=lastDoorOpenFlag)
          {
              lastDoorOpenFlag=doorOpenFlag; 
              openStateChangedFlag=true;
          }
       }

       //fprintf(logFile,"Len: %d Addr: %d  Reading: %f OpenFlag: %d \n",lastPayloadLen,thermNum,reading,doorOpenFlag);
       sprintf(postData,"{'DeviceName':'Therm%d','DeviceDate':'%ld','DeviceData1':'%f'}",thermNum,seconds_past_epoch,reading);
       callWebAPI(url,authHeader);

       if(doorOpenFlag<0 || openStateChangedFlag==false)
       {
          fprintf(logFile,"*");  //temp only
       }
       if(doorOpenFlag==0 && openStateChangedFlag)
       {
          fprintf(logFile,"C");  //garage open + temp
          sprintf(postData,"{'DeviceName':'Door%d','DeviceDate':'%ld','DeviceData1':'C'}",thermNum,seconds_past_epoch);
          callWebAPI(url,authHeader);
       }
       if(doorOpenFlag==1 && openStateChangedFlag)
       {
          fprintf(logFile,"O");  //garage closed + temp
          sprintf(postData,"{'DeviceName':'Door%d','DeviceDate':'%ld','DeviceData1':'O'}",thermNum,seconds_past_epoch);
          callWebAPI(url,authHeader);
       }

       fflush(logFile);
    }

    //Unexpected??
    if(pipeNo<1 || pipeNo>3)
    {
       timeStamp(stderr);
       fprintf(stderr, "ERROR: Unexpected pipe number: %d\n",pipeNo);
       fflush(stderr);
       retVal=-1;
    }

    //fprintf(logFile,"DATA: %s\n",postData);
    //fflush(logFile);  

    return retVal;
}

void readDeviceData()
{
   // Clear any unused ACK payloads	     
   radio.flush_tx();							 	
  				  
   //Read now
   lastPayloadLen = radio.getDynamicPayloadSize();
   //fprintf(logFile,"Now reading %d bytes on pipe: %d\n",lastPayloadLen ,pipeNo);
   //fflush(logFile);  
   radio.read(bytesRecv,lastPayloadLen);

   // Since this is a call-response. Respond directly with an ack payload.
   // Ack payloads are much more efficient than switching to transmit mode to respond to a call
   radio.writeAckPayload(pipeNo,&pipeNo,1);   
}

void callWebAPI(char *urlRaw,char *authHeader)
{
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) 
  {
     //custom headers
     struct curl_slist *chunk = NULL;
     chunk = curl_slist_append(chunk, "Accept:");
     chunk = curl_slist_append(chunk, "Content-Type: application/json;type=entry;charset=utf-8");  
     chunk = curl_slist_append(chunk, authHeader);  
     res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

    //verbose
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    //post data
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);

    //set timeouts
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15);  //set timeout for 15 seconds

    //set URL
    curl_easy_setopt(curl, CURLOPT_URL, urlRaw);
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);

    /* Check for errors */ 
    if(res != CURLE_OK)
    {
      timeStamp(stderr);
      fprintf(stderr, "ERROR: curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
      fflush(stderr);  
    }
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
    curl_slist_free_all(chunk);
  }
}

/*This function will build new auth header -- fills in authHeader*/
void createAuthHeader (char *key,char *keyName,char *url,char *authHeader)
{
    // Time past epoch
    time_t seconds_past_epoch = time(0);
    //fprintf(logFile,"Time: %ld\n",seconds_past_epoch);
    long expiry = seconds_past_epoch + 7200;
   
    // The key to hash with
    //char *key = (char *)"3RdEfuPG4TeMbsBRhLgaCnyoq5ZttZpJWFdxajN0rZM=";
    //fprintf(logFile,"Key: %s Len: %d\n",key,strlen(key));

    // The data that we're going to hash using HMAC
    //const char *url = "homeautomation-ns.servicebus.windows.net";
    char *stringToSign=(char*)malloc(256);
    sprintf(stringToSign,"%s\n%ld",url,expiry);
    //fprintf(logFile,"To Sign: %s\n",stringToSign);
    
    // Using sha256 hash engine here.
    unsigned int digLen;
    unsigned char *digest = (unsigned char*)malloc(EVP_MAX_MD_SIZE);
    HMAC(EVP_sha256(), key, strlen(key), (unsigned char*)stringToSign, strlen(stringToSign), digest, &digLen);    
    //fprintf(logFile,"HMAC digest: %s  Len: %d  Max Size: %d\n", digest,digLen,EVP_MAX_MD_SIZE);
 
    //base64 encoding
    int base64Len=0;
    char *base64data = base64encode(digest, digLen, &base64Len);  //Base-64 encodes data.
    //fprintf(logFile,"Base64 digest: %s Len: %d\n",base64data,base64Len);

    //ESC the digest
    CURL *curl = curl_easy_init();
    char *escDigest = curl_easy_escape(curl,(const char*)base64data,base64Len);
    //fprintf(logFile,"ESC'd digest: %s\n",escDigest);

    //create auth header 
    sprintf(authHeader,"Authorization: SharedAccessSignature sr=%s&sig=%s&se=%ld&skn=%s",url,escDigest,expiry,keyName);

    //free memory
    curl_easy_cleanup(curl);
    curl_free(escDigest);
    free(base64data);
    free(stringToSign);
    free(digest);
}

/*This function will Base-64 encode your data.*/
char * base64encode (const void *b64_encode_me, int encode_this_many_bytes,int *resLen)
{
    BIO *b64_bio, *mem_bio;   //Declare two BIOs.  One base64 encodes, the other stores memory.
    BUF_MEM *mem_bio_mem_ptr; //Pointer to the "memory BIO" structure holding the base64 data.

    b64_bio = BIO_new(BIO_f_base64());  //Initialize our base64 filter BIO.
    mem_bio = BIO_new(BIO_s_mem());  //Initialize our memory sink BIO.
    b64_bio = BIO_push(b64_bio, mem_bio);  //Link the BIOs (i.e. create a filter-sink BIO chain.)
    BIO_set_flags(b64_bio, BIO_FLAGS_BASE64_NO_NL);  //Don't add a newline every 64 characters.

    BIO_write(b64_bio, b64_encode_me, encode_this_many_bytes); //Encode and write our b64 data.
    BIO_flush(b64_bio);  //Flush data.  Necessary for b64 encoding, because of pad characters.
    BIO_get_mem_ptr(b64_bio, &mem_bio_mem_ptr);

    char *resBuffer = (char *)malloc(mem_bio_mem_ptr->length);
    memcpy(resBuffer , mem_bio_mem_ptr->data, mem_bio_mem_ptr->length);
    //resBuffer[mem_bio_mem_ptr->length] = 0;
    *resLen=mem_bio_mem_ptr->length;

    BIO_free_all(b64_bio);
    return resBuffer;
}


