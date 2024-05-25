#define BLYNK_TEMPLATE_NAME "TDSDHT11 Sensor"
#define BLYNK_TEMPLATE_ID "TMPL6P-Wygrt1"
#define BLYNK_AUTH_TOKEN "acPxQmZF_0swYPa7ANkaA9ZkzybGgcyj"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define DHTPIN D1       // Define the pin where the DHT sensor is connected
#define DHTTYPE DHT11   // Define the type of DHT sensor

DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "HOME65_2.4Gz";
char pass[] = "59454199";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  
  int Temperature = dht.readTemperature();
  int Humidity = dht.readHumidity();
  
  if (isnan(Temperature) || isnan(Humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Serial.print("Humidity: ");
  Serial.print(Humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.println(Temperature);
  
  Blynk.virtualWrite(V2, Temperature);
  Blynk.virtualWrite(V3, Humidity);
  
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
}
