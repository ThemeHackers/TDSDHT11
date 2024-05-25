Markdown
## TDSDHT11: Water Quality Temperature and Humidity Sensors

**TDSDHT11** is an easy-to-use and inexpensive temperature and humidity sensor that is perfect for a variety of projects, such as measuring the temperature and humidity in a fish tank, a home temperature and humidity control system, or even measuring the temperature and humidity in the environment.

**Features:**

* Measures temperature and humidity accurately
* Easy to use
* Inexpensive
* Small and compact
* Low power consumption

**Documentation:**

* TDSDHT11 Datasheet: [https://www.adafruit.com/product/386](https://www.adafruit.com/product/386)
* Adafruit_DHT_Unified Library (Python): [https://github.com/adafruit/Adafruit_Python_DHT](https://github.com/adafruit/Adafruit_Python_DHT)
* TDSDHT11 Example Usage (similar for Arduino): [https://www.adafruit.com/product/385](https://www.adafruit.com/product/385)

**Installation (for Arduino):**

1. Install the Adafruit_DHT_Unified library (refer to your Arduino IDE library manager for installation instructions).

**Code Example (Arduino):**

```c++
#include <Adafruit_DHT.h>

#define DHTPIN 4 // Pin number connected to the TDSDHT11 sensor
#define DHTTYPE DHT11 // Type of TDSDHT11 sensor

Adafruit_DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
}

void loop() {
  float temperature, humidity;
  dht.readSensor(&temperature, &humidity);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C ");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  delay(2000);
}
