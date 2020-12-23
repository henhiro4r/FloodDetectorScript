#include <ESP8266WiFi.h>
#include <SimpleTimer.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#define FIREBASE_HOST "flooddetector-92b5e-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "gYbg1i1ZCKXjFIq52ZZ2D4qTgI3gVpAacvw1FFFO"

#define WIFI_SSID "ffs"
#define WIFI_PASSWORD "123456789"

const int triggerPin = D6; // Trigger Pin of Ultrasonic Sensor
const int echoPin = D7; // Echo Pin of Ultrasonic Sensor
long duration, distance;

SimpleTimer timer;

WiFiServer server(80);

void setup() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.begin(115200);
  
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  timer.setInterval(3000);
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected");

  server.begin();
  Serial.println("Server Connected");

  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  if  (timer.isReady()) {
    Serial.println();
    pinMode(triggerPin, OUTPUT);
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = microsecondsToCentimeters(duration);
    
    if  (distance <= 15) {
      String currLevel = String(15 - distance);
    
      Firebase.setString("currlevel", currLevel);
      
      if  (Firebase.failed()) {
        Serial.print("Set value failed");
        Serial.println(Firebase.error());
        delay(500);
        return;
      }
    }

    Firebase.setString("currlevel", "0");
    
    Serial.print(Firebase.getString("currlevel"));
    Serial.println();
    timer.reset();
  }
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 58.2;
}
