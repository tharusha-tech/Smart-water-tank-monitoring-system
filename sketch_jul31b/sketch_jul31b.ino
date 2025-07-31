#include <WiFi.h>
#include <WebServer.h>

// WiFi credentials
const char* ssid = "M14";           // <<== hotspot නම
const char* password = "12345678";   // <<== hotspot මුරපදය

#define RELAY_PIN 27

WebServer server(80);  // Web server on port 80

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Start with Relay OFF

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected! IP Address:");
  Serial.println(WiFi.localIP());

  // Define relay control endpoints
  server.on("/relay/on", []() {
    digitalWrite(RELAY_PIN, HIGH);
    server.send(200, "text/plain", "Relay turned ON");
    Serial.println("Relay turned ON");
  });

  server.on("/relay/off", []() {
    digitalWrite(RELAY_PIN, LOW);
    server.send(200, "text/plain", "Relay turned OFF");
    Serial.println("Relay turned OFF");
  });

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}