#include <ESP8266WiFi.h>
#include <Wire.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "iotex-fad35.firebaseio.com"                 
#define FIREBASE_AUTH "jCNlnhNqBBQ93rdPwbILfiuf01FOziAdYrDdXwGr"    

#define WIFI_SSID "ayush_vivo"                                             
#define WIFI_PASSWORD "17113650"                                    

int countYes = 0;
int carEnter = D0;                    
int carExited = D4;  
int status;                 
int TRIG = D7;                  
int ECHO = D8;                
int led = D3;                 
int pos;
int pos1;

long duration, distance;     

void setup() {
  delay(1000);
  Serial.begin (9600);     
  pinMode(TRIG, OUTPUT);       
  pinMode(ECHO, INPUT);         
  pinMode(led, OUTPUT);         
  pinMode(carExited, INPUT);    
  pinMode(carEnter, INPUT);     

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);                          
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");                          
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);        

}

void loop() {

  digitalWrite(TRIG, LOW);         // make trig pin low
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);        // make trig pin high
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration / 2) / 29.1;      // take distance in cm
    
    Serial.print("Centimeter: ");
    Serial.println(distance);
    delay(500);

  int carEntry = digitalRead(carEnter);      
  if (carEntry == HIGH) {                      
    countYes++;                             
    Serial.print("Car Entered = " ); Serial.println(countYes );
    delay(2000);    
    
  }
  int carExit = digitalRead(carExited);            
  if (carExit == HIGH) {                           
    countYes--;                                    
    Serial.print("Car Exited = " ); Serial.println(countYes);

    //Firebase.setString("Parking Status/Space", fireAvailable );  
    delay(2000);
  }
  if (distance < 15) {                      
        Serial.println("Occupied ");
        digitalWrite(led, HIGH);
        delay(2000);
        status=1;
        Firebase.setInt("Parking Status/Space", status);
  }

  if (distance > 15) {                        
        Serial.println("Available ");
        digitalWrite(led, LOW);
        delay(2000);
        status=0;
        Firebase.setInt("Parking Status/Space", status);
  }


  delay(500);
}
