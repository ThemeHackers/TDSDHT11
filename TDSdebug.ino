#define BLYNK_TEMPLATE_NAME "TDSDHT11 Sensor"
#define BLYNK_TEMPLATE_ID "TMPL6P-Wygrt1"
#define BLYNK_AUTH_TOKEN "acPxQmZF_0swYPa7ANkaA9ZkzybGgcyj"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WebServer.h>
#include "GravityTDS.h"

// Default WiFi credentials
char ssid[][32] = {"HOME65_2.4Gz", "@TPN_Teacher_WiFi", "@TPN_POOL", "TOP"};
char pass[][64] = {"59454199", "@tpn42566", "t12345678t", "44554455"};
int numNetworks = 4; // Number of networks to try

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
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid[0], pass[0]);

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
    // Serial.println("WiFi connection lost. Reconnecting...");

    for (int i = 0; i < numNetworks; i++) {
      // Serial.print("Trying SSID: ");
      // Serial.println(ssid[i]);
      WiFi.begin(ssid[i], pass[i]);

      int attempts = 0;
      while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        // Serial.print(".");
        attempts++;
      }

      if (WiFi.status() == WL_CONNECTED) {
        // Serial.println("\nWiFi connected to: " + String(ssid[i]));
        break; // Exit the loop if connected successfully
      } else {
        // Serial.println("\nWiFi connection failed. Trying next network...");
        WiFi.disconnect();
        delay(1000);
      }
    }
  }
}

// Blynk function to handle terminal input
BLYNK_WRITE(V4) {
  String command = param.asStr();

  if (command == "ipconfig") {
    String ipAddress = WiFi.localIP().toString();
    terminal.println("IP Address to reset: http://" + ipAddress + "/reset");
    terminal.flush();
  } else if (command == "clear") {
    terminal.clear();
    terminal.println("Terminal cleared...");
    terminal.flush();
  } else if (command.startsWith("ssid:")) {
    command.remove(0, 5);
    command.toCharArray(ssid[0], sizeof(ssid[0]));
    terminal.println("SSID set to: " + String(ssid[0]));
    terminal.flush();
  } else if (command.startsWith("pass:")) {
    command.remove(0, 5);
    command.toCharArray(pass[0], sizeof(pass[0]));
    terminal.println("Password set to: " + String(pass[0]));
    terminal.flush();
  } else if (command == "save") {
    terminal.println("Saving new WiFi credentials and restarting...");
    terminal.flush();
    delay(100);
    ESP.restart();
  } else {
    terminal.println("Unknown command: " + command);
    terminal.flush();
  }
}
