#include <ESP8266WiFi.h>
#include <SimpleTimer.h>

char ssid[] = "ffs";
char password[] = "123456789";

const int triggerPin = D6; // Trigger Pin of Ultrasonic Sensor
const int echoPin = D7; // Echo Pin of Ultrasonic Sensor
long duration, distance;

SimpleTimer timer;

WiFiServer server(80);

void setup() {
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  timer.setInterval(3000);
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
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
    Serial.print(distance);
    Serial.print(" cm");
    Serial.println();
    timer.reset();
  }
}
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 58.2;
}
