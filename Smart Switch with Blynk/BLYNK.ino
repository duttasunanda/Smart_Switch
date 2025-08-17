/*************************************************************
   Control Bulb with ESP8266 + Blynk + Relay
 *************************************************************/

#define BLYNK_TEMPLATE_ID "TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "ZenInfiny Smart Light"
#define BLYNK_AUTH_TOKEN "XXXX"

// Include libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Enter your WiFi credentials
char ssid[] = "IOT";      // WiFi SSID
char pass[] = "12345678";  // WiFi Password

// Relay pin connected to ESP8266 (D1 pin for example)
#define RELAY_PIN 2  

// Auth Token from Blynk
char auth[] = BLYNK_AUTH_TOKEN;

// Blynk virtual pin
#define VPIN V1  

BLYNK_WRITE(VPIN) {
  int relayState = param.asInt(); // Get value from Blynk app (0 or 1)
  digitalWrite(RELAY_PIN, relayState);
}

void setup() {
  // Start serial for debugging
  Serial.begin(9600);

  // Initialize relay pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Keep relay off at start

  // Connect to Blynk
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run(); // Run Blynk background tasks
}

