#include <ESP8266WiFi.h>
#include <Servo.h>
#include <Wire.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST ""                 
#define FIREBASE_AUTH ""            
#define WIFI_SSID ""                                            
#define WIFI_PASSWORD ""                                   
      
Servo myservo;                          
Servo myservos;                               
int status;
int countYes = 0;
int carEnter = D0;                      
int carExited = D4;                  
int TRIG = D7;                  
int ECHO = D8;                
int led = D3;                
int pos;
int pos1;

long duration, distance;     

void setup() {
  delay(1000);
  Serial.begin (9600);     
  //Wire.begin(D2, D1);       
  myservo.attach(D6);      
  myservos.attach(D5);      
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

  digitalWrite(TRIG, LOW);         
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);        
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration / 2) / 29.1;      

    Serial.print("Centimeter: ");
    Serial.println(distance);

  int carEntry = digitalRead(carEnter);      
  if (carEntry == HIGH) {                    
    countYes++;                             
    Serial.print("Car Entered = " ); Serial.println(countYes );
    //lcd.setCursor(0, 1);
    //lcd.print("Car Entered");
    for (pos = 140; pos >= 45; pos -= 1) {        // change servo position
      myservos.write(pos);                       
      delay(10);
    }
    
    delay(2000);

    for (pos = 45; pos <= 140; pos += 1) {       
      // in steps of 1 degree
      myservos.write(pos);
      delay(10);
    }

    
  }
  int carExit = digitalRead(carExited);              
  if (carExit == HIGH) {                            
    countYes--;                                    
    Serial.print("Car Exited = " ); Serial.println(countYes);
 
    for (pos1 = 140; pos1 >= 45; pos1 -= 1) {         
      myservo.write(pos1);
      delay(5);
    }
    
    delay(2000);

   for (pos1 = 45; pos1 <= 140; pos1 += 1) {            
      
      myservo.write(pos1);
      delay(5);
    }
    

  }
  if (distance < 15) {                      
        Serial.println("Occupied ");
        digitalWrite(led, HIGH);
        status=1;
        Firebase.setInt("Parking Status/Space", status);
       
        delay(2000);
  }

  if (distance > 15) {                        
        Serial.println("Available ");
        digitalWrite(led, LOW);
        status=0;
        Firebase.setInt("Parking Status/Space", status);
     
        delay(2000);
  }

  delay(2000);
}
