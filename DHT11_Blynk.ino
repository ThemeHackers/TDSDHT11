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

char ssid[] = "HOME65_2.4Gz";
char pass[] = "59454199";

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
  } else {
    terminal.println("Unknown command: " + command);
    terminal.flush();
  }
}
