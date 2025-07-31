#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "M14";           // <<== ඔබේ hotspot නම
const char* password = "12345678";   // <<== ඔබේ hotspot password

const char* receiverIP = "10.86.130.130";  // RECEIVER ESP32 IP address (fixed or known)

#define TRIG_PIN 5
#define ECHO_PIN 18

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected! IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Trigger ultrasonic pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure duration
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = (duration * 0.0343) / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Send HTTP request based on distance
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url;

    if (distance > 0 && distance <= 7) {  // ← 3cm අඩු කළ limit
      url = "http://" + String(receiverIP) + "/relay/on";
    } else {
      url = "http://" + String(receiverIP) + "/relay/off";
    }

    http.begin(url);
    int httpCode = http.GET();
    http.end();

    Serial.print("Request sent to: ");
    Serial.println(url);
    Serial.print("Response code: ");
    Serial.println(httpCode);
  } else {
    Serial.println("WiFi disconnected!");
  }

  delay(1000); // wait 1 second
}
