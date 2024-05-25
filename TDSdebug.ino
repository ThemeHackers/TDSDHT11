#define BLYNK_TEMPLATE_NAME "TDSDHT11 Sensor"
#define BLYNK_TEMPLATE_ID "TMPL6P-Wygrt1"
#define BLYNK_AUTH_TOKEN "acPxQmZF_0swYPa7ANkaA9ZkzybGgcyj"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
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
}

void loop() {
  Blynk.run();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Update the TDS sensor reading
    gravityTds.setTemperature(temperature);
    gravityTds.update();
    tdsValue = gravityTds.getTdsValue();

    // Print TDS value to serial monitor
    Serial.print("TDS: ");
    Serial.print(tdsValue, 0);
    Serial.println(" ppm");

    if (tdsValue > 18) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
    }

    if (tdsValue > 500) {
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
