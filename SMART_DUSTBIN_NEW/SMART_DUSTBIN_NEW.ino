#include <Servo.h>
#include <WiFi.h>
//#include "secrets.h"
#include <ThingSpeak.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

char ssid[50] = "#123";    //  your network SSID (name) 
char pass[50] = "sonal237";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = 1880569;
const char * myWriteAPIKey = "OI2L5I20EMVDZ3ZT";

Servo servo1;
Servo servo2;

//#define main_trigpin 2
//#define main_echopin 3 
#define wet_trigpin 4
#define wet_echopin 5  
#define dry_trigpin 6 
#define dry_echopin 7
#define LED 9
#define load_data 12
#define load_clock 13
const int waterSens = A5;
int pos = 0;//define servo position

//long main_duration;  
//int main_distance; 
long wet_duration;  
int wet_distance; 
long dry_duration;  
int dry_distance; 
//int water_value = 0;


void setup() 
{
  Serial.begin(9600);

  servo1.attach(10);//dry
  servo1.write(0);
  servo2.attach(11);              //wet
  servo2.write(0); 
  pinMode(wet_trigpin,OUTPUT);  
  pinMode(wet_echopin,INPUT); 
  pinMode(dry_trigpin,OUTPUT);  
  pinMode(dry_echopin,INPUT);  
//pinMode(water_powerpin, OUTPUT);  
  //digitalWrite(water_powerpin, LOW);   //wanter sensor off

  if (WiFi.status() == WL_NO_SHIELD) 
  {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0") 
  {
    Serial.println("Please upgrade the firmware");
  }
    
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() 
{
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  
 /* digitalWrite(main_trigpin,HIGH);  
  delayMicroseconds(10);  
  digitalWrite(main_trigpin,LOW);  
  main_duration=pulseIn(main_echopin,HIGH);  
  main_distance = main_duration*0.034/2; 
*/
    digitalWrite(wet_trigpin,HIGH);      //ultrasonic sensor wet side
    delayMicroseconds(10);  
    digitalWrite(wet_trigpin,LOW);  
    wet_duration=pulseIn(wet_echopin,HIGH);  
    wet_distance = wet_duration*0.034/2; 
  
    digitalWrite(dry_trigpin,HIGH);     //ultrasonic sensor dry side
    delayMicroseconds(10);  
    digitalWrite(dry_trigpin,LOW);  
    dry_duration=pulseIn(dry_echopin,HIGH);  
    dry_distance = dry_duration*0.034/2; 

  
    int sensorValue = analogRead(waterSens);    //read the water sensor value
    sensorValue = map(sensorValue, 0, 1023, 0, 180);    

     if (sensorValue >= 50) 
     {
        servo1.write(0);
        for (pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees in steps of 1 degree
        { 
          servo2.write(pos);              // tell servo to go to position in variable 'pos'
          delay(15);                       // waits 15ms for the servo to reach the position
        }

        for (pos = 180; pos >= 0; pos -= 1)  // goes from 180 degrees to 0 degrees
        {    
          servo2.write(pos);              // tell servo to go to position in variable 'pos'
          delay(15);                       // waits 15ms for the servo to reach the position
        }
      }
 
    else
    {
      servo2.write(0);
      for (pos = 0; pos <= 180; pos += 1) 
      { 
        servo1.write(pos);              
        delay(15);                      
      }

    for (pos = 180; pos >= 0; pos -= 1) 
    { 
      servo1.write(pos);              
      delay(15);                       
    }
  }
 
  Serial.println(sensorValue);
  delay(20);        

  if(wet_distance==25 || dry_distance==25)
  {
      digitalWrite(LED,HIGH);
      if(wet_distance>dry_distance)
      {
       ThingSpeak.setField(1, wet_distance);
       ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      }
      else if(dry_distance>wet_distance)
      {
       ThingSpeak.setField(1, dry_distance);
       ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      }
  }

}
