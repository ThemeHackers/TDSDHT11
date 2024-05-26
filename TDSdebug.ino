#define BLYNK_TEMPLATE_NAME "TDSDHT11 Sensor"
#define BLYNK_TEMPLATE_ID "TMPL6P-Wygrt1"
#define BLYNK_AUTH_TOKEN "acPxQmZF_0swYPa7ANkaA9ZkzybGgcyj"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WebServer.h>
#include "GravityTDS.h"

// WiFi credentials
char ssid[] = "HOME65_2.4Gz";
char pass[] = "59454199";

// Pin definitions
#define TdsSensorPin A0
#define redPin D0
#define greenPin D1
#define bluePin D2

// GravityTDS instance
GravityTDS gravityTds;

float temperature = 25, tdsValue = 0;
unsigned long previousMillis = 0;
const long interval = 1000; // Interval for updating TDS value and Blynk

// Blynk terminal widget on virtual pin V4
WidgetTerminal terminal(V4);

// Web server on port 80
ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "Hello! Go to /reset to reset the device.");
}

void handleReset() {
  server.send(200, "text/plain", "Device is resetting...");
  delay(100);
  ESP.restart();
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Configure pin modes
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize GravityTDS sensor
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);
  gravityTds.setAdcRange(1024);

  // Initialize Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Initialize web server
  server.on("/", handleRoot);
  server.on("/reset", handleReset);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  Blynk.run();
  server.handleClient();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Update the TDS sensor reading
    gravityTds.setTemperature(temperature);
    gravityTds.update();
    tdsValue = gravityTds.getTdsValue();

    // Print TDS value to serial monitor
    // Serial.print("TDS: ");
    // Serial.print(tdsValue, 0);
    // Serial.println(" ppm");

    if (tdsValue > 18) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
    }

    if (tdsValue > 100) {
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
    } else {
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, LOW);
    }

    // Send TDS value to Blynk
    Blynk.virtualWrite(V0, tdsValue);
  }

  // Check WiFi connection periodically
  checkWiFiConnection();
}

void blinkBuiltinLED(int onTime, int offTime) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(onTime);
  digitalWrite(LED_BUILTIN, LOW);
  delay(offTime);
}

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    WiFi.begin(ssid, pass);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      Serial.print(".");
      attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWiFi reconnected");
    } else {
      Serial.println("\nWiFi reconnection failed. Please check your credentials.");
    }
  }
}

// Blynk function to handle terminal input
BLYNK_WRITE(V4) {
  String command = param.asStr();

  if (command == "ipconfig") {
    String ipAddress = WiFi.localIP().toString();
    terminal.println("IP Address: http://" + ipAddress + "/reset");
    terminal.flush();
  } else if (command == "clear") {
    terminal.clear();
    terminal.println("Terminal cleared...");
    terminal.flush();
  } else {
    terminal.println("Unknown command: " + command);
    terminal.flush();
  }
}
