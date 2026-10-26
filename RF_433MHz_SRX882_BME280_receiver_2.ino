
//RF-433MHz-SRX882-BME280-receiver-2
//upgrade of RF-433MHz-SRX882-BME280-receiver-1
//realised actual on the Arduino Nano as RF433 scanner
//button is on D%, LedRfPin green is on D4, RF available  is on D2
/*
 Sketch for reading of data from BME280 sensor with RF433MHz technology
Data from RF transmitter are formed with code for specification of different data
Example : specialised code for temperature is here set to  10 000 000
          and to this code is added temperature (example) 23.45 * 100 = 2345
          if temperature < 0 , the special code is set to 11 000 000
          special code for humidity is set  to 12 000 000
          special code for air press is set to 13 000 000
This RF433 MHz application is based on library and examles 
from  https://github.com/sui77/rc-switch/
Reprogrammed by PavelOu
transformed from ReceiveDemo_Simple-po
and next transformed from RF433-OT-semafor-receiver-po-7
version for Arduino Nano Clone
revisioned from RF-433MHz-SRX882-BME280-receiver
yellow LED for incoming RF data only
new green LED for data are ready
new red Led for errors in RF
some changes in timings
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

//static int rfPin = 2; //default ISR (Interupt) for Arduinos

String sketchName = "RF-433MHz-SRX882-BME280-receiver-2";

unsigned int *raw;

const int ledPinRF = 6; // yellow Led for RF receive of new data
//const int ledPinDataOn = 7; // green Led for RF received of valid data
const int ledPinRFerror = 8; // yellow Led for RF receive of new data

const int ledPinDataOn = 4; // yellow Led for RF receive of new data

bool bmeDataOn = false;

byte protocolNr = 2;  //must be same as in transmitter
byte nrBits = 32;     // - '' -
long int specialCode = 1000000;  // - '' -
long int rfData = 0;
long int rfUnit = 0;
long int rfPom = 0;

unsigned long decimal;
unsigned int lengthOfWord;
unsigned int delayOfPulse;
unsigned int protocol; 


byte dataMode = 0;
bool tempOn = false;
bool humpOn = false;
bool prespOn = false;

float temp = 0;
float hump = 0;
float presp = 0;

void setup() 
 {
  Serial.begin(9600);
  while (!Serial) 
   {
     delay(1000);     
     // wait for serial port to connect. Needed for native USB port only
   }
  delay(1000);
  pinMode(ledPinDataOn,OUTPUT);
  pinMode(ledPinRF,OUTPUT);
  pinMode(ledPinRFerror,OUTPUT);
  digitalWrite(ledPinRF,HIGH);
  delay(1000);
  digitalWrite(ledPinRF,LOW);
  digitalWrite(ledPinDataOn,HIGH);
  delay(1000);
  digitalWrite(ledPinDataOn,LOW);
  digitalWrite(ledPinRFerror,HIGH);                        
  delay(1000);
  digitalWrite(ledPinRFerror,LOW);                          
  Serial.println(sketchName);
  Serial.println("RF 433 Mhz  receiver of telemetry data");

  dataMode = 0;
  bmeDataOn = false;
  tempOn = false;
  humpOn = false;
  prespOn = false;

  protocolNr = 2;
  nrBits = 32; 

  // Receiver on interrupt 0 => that is pin #2 for Arduina
  // Receiver on interrupt 0 => that is pin GPIO00 as D3 in ESP8266
  mySwitch.enableReceive(0);  
 }

void loop() 
 {
  if (mySwitch.available())
        {
           //digitalWrite(ledPinRF,HIGH);                                              
           decimal = mySwitch.getReceivedValue();
           delayOfPulse = mySwitch.getReceivedDelay();
           lengthOfWord = mySwitch.getReceivedBitlength();
           protocol = mySwitch.getReceivedProtocol();
           mySwitch.getReceivedRawdata();
           mySwitch.resetAvailable();
           /*
           Serial.println();                         
           Serial.print(" Protocol: "); //for test only
           Serial.println(protocol);
           Serial.print("Decimal: "); 
           Serial.println(decimal);
           Serial.print("Bits: ");
           Serial.print(lengthOfWord);
           Serial.print(" PulseLength: ");
           Serial.print(delayOfPulse);
           Serial.print(" microseconds");
           Serial.println();
           */
           /*         
           Serial.print("Raw data: ");
           for (unsigned int i=0; i<= lengthOfWord*2; i++) 
                {
                  Serial.print(raw[i]);
                  Serial.print(",");
                }
            Serial.println();
            */
            if (!bmeDataOn)
              {
               dataMode = 1;
               //Serial.println("RF data On !");
              }
            //delay(250);
            //digitalWrite(ledPinRF,LOW);                                
            //delay(250);                
        }       
 if (dataMode == 1) 
  {
    if (!bmeDataOn)
     {
      //Serial.println("RF433 control of received data: "); // for test only
      dataMode = 2;
     }
    else
     {  
       dataMode = 0;
     }  
   } //end of dataMode = 1   
   
  if (dataMode == 2)
   {
    if (protocol != protocolNr)
     {
        //Serial.println("Another protocol !");   //for test only
        //Serial.println("This is not command for my semafor !");
        //Serial.println();
        digitalWrite(ledPinRF,LOW);                        
        digitalWrite(ledPinRFerror,HIGH);                        
        delay(500);
        digitalWrite(ledPinRFerror,LOW);                        
        dataMode = 0;
      }
    else
     {
      if (lengthOfWord != nrBits)   
       {
        //Serial.println("Another bits length !"); //for test only
        //Serial.println("This is not command for my semafor ! !");
        //Serial.println();
        digitalWrite(ledPinRF,LOW);                        
        digitalWrite(ledPinRFerror,HIGH);                        
        delay(500);
        digitalWrite(ledPinRFerror,LOW);                        
      dataMode = 0; 
       }
     }
  }
 if (dataMode == 2) 
  {
    // decimal example = 12002340  specialCode = 1000000
    rfUnit = decimal / specialCode; // 12002345 / 1000000 = 12
    //Serial.print("Result unit: ");
    //Serial.println(rfUnit);
    rfPom = rfUnit * specialCode; // 12 * 1000000 = 12000000
    //Serial.print("Result pom: ");
    //Serial.println(rfPom);
    rfData = decimal - rfPom; // 12002345 - 12000000 = 2345
    //Serial.print("Result data: ");
    //Serial.println(rfData);
    switch(rfUnit)
     {
      case 10:
       if (!tempOn)
        {
          //digitalWrite(ledPinRF,HIGH);    
          temp = rfData/100.0;
          //Serial.println("Temp LED On !");   // for test only     
          //Serial.print("Temp = ");
          //Serial.println(temp);
          //delay(500); 
          if (temp < 101)
           {
            tempOn = true;            
           }
          else
           {
            digitalWrite(ledPinRF,LOW);  
            digitalWrite(ledPinRFerror,HIGH);                        
            delay(500);
            digitalWrite(ledPinRFerror,LOW);                          
           }
          humpOn = false;
          prespOn = false;
        }  
       break;       

      case 11:
       if (!tempOn)
        {
          float tempPom = rfData/100.0;
          if (tempPom < 100)
           {
            temp = -1*tempPom;
            tempOn = true;            
           }
          else
           {
            digitalWrite(ledPinRF,LOW);                        
            digitalWrite(ledPinRFerror,HIGH);                        
            delay(500);
            digitalWrite(ledPinRFerror,LOW);                          
           }
          //Serial.println("Temp LED On !");   // for test only     
          //Serial.print("Temp = ");
          //Serial.println(temp);
          //delay(500); 
          humpOn = false;
          prespOn = false;
        }  
       break;       

      case 12:
       if (!humpOn)
        {
         hump = rfData/100.0;          
         //Serial.println("Hump On !"); //for test only
         //Serial.print("Hump=");
         //Serial.println(hump);
         //delay(500); 
         humpOn = true;
         prespOn = false;
        } 
       break; 

      case 13:
       if (!prespOn)
        {
         presp = rfData/100.0;         
         //Serial.println("Presp On !");  //for test only
         //Serial.print("Presp=");
         //Serial.println(presp);
         //delay(500); 
         prespOn = true; 
        } 
       break; 
       
      default:
       dataMode = 0;
       digitalWrite(ledPinRF,LOW);                                
       digitalWrite(ledPinRFerror,HIGH);                        
       delay(500);
       digitalWrite(ledPinRFerror,LOW);                        
       //delay(500);
     }
    //Serial.println(); // for test only
    if (tempOn&&humpOn&&prespOn)
     {
      //Serial.println("All RF data On !");
      digitalWrite(ledPinRF,LOW);
      digitalWrite(ledPinDataOn,HIGH);
      bmeDataOn = true;
      dataMode = 0;     
      Serial.print("*");
      Serial.print(temp);
      Serial.print(",");
      Serial.print(hump);
      Serial.print(",");
      Serial.print(presp);
      Serial.println(",#,");
      tempOn = false;
      humpOn = false;
      prespOn = false;
      //Serial.println();
      delay(2000);
      digitalWrite(ledPinDataOn,LOW);
      bmeDataOn = false;
     }
   }  
 }
