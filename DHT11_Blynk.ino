#define BLYNK_TEMPLATE_NAME "TDSDHT11 Sensor"
#define BLYNK_TEMPLATE_ID "TMPL6P-Wygrt1"
#define BLYNK_AUTH_TOKEN "acPxQmZF_0swYPa7ANkaA9ZkzybGgcyj"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTPIN D1       // Define the pin where the DHT sensor is connected
#define DHTTYPE DHT11   // Define the type of DHT sensor

DHT dht(DHTPIN, DHTTYPE);

char ssid[][32] = {"HOME65_2.4Gz", "@TPN_Teacher_WiFi", "@TPN_POOL", "TOP"};
char pass[][64] = {"59454199", "@tpn42566", "t12345678t", "44554455"};
int numNetworks = 4; // Number of networks to try

// Blynk terminal widget on virtual pin V5
WidgetTerminal terminal(V5);

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
}

void loop() {
  Blynk.run();
  server.handleClient();
  
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
  
  Blynk.virtualWrite(V2, temperature);
  Blynk.virtualWrite(V3, humidity);
  
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
}

void connectToWiFi() {
  for (int i = 0; i < numNetworks; i++) {
    Serial.print("Connecting to SSID: ");
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

// Blynk function to handle terminal input
BLYNK_WRITE(V5) {
  String command = param.asStr();

  if (command == "ipconfig") {
    String ipAddress = WiFi.localIP().toString();
    terminal.println("IP Address: http://" + ipAddress + "/reset");
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
