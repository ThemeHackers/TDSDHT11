#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

// Define pin and type for DHT sensor
#define DHTPIN D1
#define DHTTYPE DHT11

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// WiFi credentials
char ssid[][32] = {"HOME65_2.4Gz", "@TPN_Teacher_WiFi", "@TPN_POOL", "TOP"};
char pass[][64] = {"59454199", "@tpn42566", "t12345678t", "44554455"};
int numNetworks = 4; // Number of networks to try

// Blynk terminal widget on virtual pin V5
WidgetTerminal terminal(V5);

// Web server on port 80
ESP8266WebServer server(80);

// Handle root path
void handleRoot() {
  server.send(200, "text/plain", "Hello! Go to /reset to reset the device.");
}

// Handle reset path
void handleReset() {
  server.send(200, "text/plain", "Device is resetting...");
  delay(100);
  ESP.restart();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  dht.begin();

  // Initialize web server
  server.on("/", handleRoot);
  server.on("/reset", handleReset);
  server.begin();
  Serial.println("HTTP server started");

  // Connect to WiFi
  connectToWiFi();

  // Initialize Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid[0], pass[0]);

  // Print IP address to Blynk terminal after connecting to WiFi
  printIPAddress();
}

void loop() {
  Blynk.run();
  server.handleClient();

  // Read temperature and humidity from DHT sensor
  int temperature = dht.readTemperature();
  int humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.println(temperature);

  // Send data to Blynk
  Blynk.virtualWrite(V2, temperature);
  Blynk.virtualWrite(V3, humidity);

  // Blink the built-in LED
  digitalWrite(LED_BUILTIN, HIGH);
  delay(150);
  digitalWrite(LED_BUILTIN, LOW);
  delay(300);

  // Check WiFi connection periodically
  checkWiFiConnection();
}

void connectToWiFi() {
  // Try to connect to all SSIDs in the list
  for (int i = 0; i < numNetworks; i++) {
    Serial.print("Connecting to SSID: ");
    Serial.println(ssid[i]);
    WiFi.begin(ssid[i], pass[i]);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(250);
      Serial.print(".");
      attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWiFi connected to: " + String(ssid[i]));
      break; // Exit the loop if connected successfully
    } else {
      Serial.println("\nWiFi connection failed. Trying next network...");
      WiFi.disconnect();
      delay(250);
    }
  }
}

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");

    for (int i = 0; i < numNetworks; i++) {
      Serial.print("Trying SSID: ");
      Serial.println(ssid[i]);
      WiFi.begin(ssid[i], pass[i]);

      int attempts = 0;
      while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
      }

      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected to: " + String(ssid[i]));
        break; // Exit the loop if connected successfully
      } else {
        Serial.println("\nWiFi connection failed. Trying next network...");
        WiFi.disconnect();
        delay(1000);
      }
    }
  }
}

void printIPAddress() {
  if (WiFi.status() == WL_CONNECTED) {
    String ipAddress = WiFi.localIP().toString();
    terminal.println("Device IP Address: " + ipAddress);
    terminal.flush();
  } else {
    terminal.println("Not connected to WiFi");
    terminal.flush();
  }
}

// Blynk function to handle terminal input
BLYNK_WRITE(V5) {
  String command = param.asStr();

  if (command == "ipconfig") {
    printIPAddress();
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
