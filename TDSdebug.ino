#define BLYNK_TEMPLATE_NAME "TDSDHT11 Sensor"
#define BLYNK_TEMPLATE_ID "TMPL6P-Wygrt1"
#define BLYNK_AUTH_TOKEN "acPxQmZF_0swYPa7ANkaA9ZkzybGgcyj"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char ssid[] = "HOME65_2.4Gz";
char pass[] = "59454199";
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include "GravityTDS.h"

#define TdsSensorPin A0

GravityTDS gravityTds;

float temperature = 25, tdsValue = 0;

const int redPin = D0;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);
  gravityTds.setAdcRange(1024);
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  gravityTds.setTemperature(temperature);
  gravityTds.update();
  tdsValue = gravityTds.getTdsValue();

  Serial.print("TDS: ");
  Serial.print(tdsValue, 0);
  Serial.println(" ppm");

  // Adjusting the conditions
  if (tdsValue >= 500) {
    digitalWrite(LED_BUILTIN, HIGH); // Red color
    digitalWrite(redPin, HIGH);
  } else if (tdsValue >= 301 && tdsValue <= 500) {
    digitalWrite(LED_BUILTIN, HIGH); // Red color
    digitalWrite(redPin, HIGH);
  } else if (tdsValue >= 50 && tdsValue <= 300) {
    digitalWrite(LED_BUILTIN, LOW); // Green color
    digitalWrite(redPin, LOW);
  } else {
    digitalWrite(LED_BUILTIN, LOW); // Turn off LED
    digitalWrite(redPin, LOW);
  }

  Blynk.virtualWrite(V0, tdsValue);

  checkWiFiConnection(); // Check WiFi connection periodically
  delay(1000);           // Adjust the delay according to your requirements
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
