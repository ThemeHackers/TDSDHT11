## TDSDHT11: Water Quality Temperature and Humidity Sensors
The microcontroller is reset via the web server http://............/reset

**TDSDHT11** is an easy-to-use and inexpensive temperature and humidity sensor that is perfect for a variety of projects, such as measuring the temperature and humidity in a fish tank, a home temperature and humidity control system, or even measuring the temperature and humidity in the environment.

**Features:**

* Measures temperature and humidity accurately
* Easy to use
* Inexpensive
* Small and compact
* Low power consumption

**Equipment required for this project:**
* Wemos D1 ESP8266
* ESP8266 NodeMCU 1.0 (ESP-12E Module)
* DHT11 sensor
* TDS sensor
* rgb sensor
* USB UART
  
**Documentation:**

* TDSDHT11 Datasheet: [https://www.adafruit.com/product/386](https://www.adafruit.com/product/386)
* Adafruit_DHT_Unified Library (Python): [https://github.com/adafruit/Adafruit_Python_DHT](https://github.com/adafruit/Adafruit_Python_DHT)
* TDSDHT11 Example Usage (similar for Arduino): [https://www.adafruit.com/product/385](https://www.adafruit.com/product/385)

**Installation (for Arduino):**

1. Install the Adafruit_DHT_Unified library (refer to your Arduino IDE library manager for installation instructions).

**Code Example (Arduino):**

```c++
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
```
**Notes:**

* This example code serves as a starting point; modify it for your specific needs.
* Ensure the TDSDHT11 sensor is connected to the correct pin on your Arduino board.
* Use the latest version of the Adafruit_DHT_Unified library.
* 
**Potential Bugs and Troubleshooting:**
  
* Library Mismatch: The code might encounter errors if the Arduino libraries on your computer differ from those used in the provided code. This can occur if you're using a different Arduino IDE version or have manually installed libraries. To resolve this, update any outdated libraries through the Arduino IDE library manager and ensure you have the latest versions installed.
* Hardware Issues: Double-check the physical connections between the TDSDHT11 sensor and the Arduino board. Verify that the sensor is securely connected to the designated pins and the wiring is secure. Faulty connections or damaged components can lead to unexpected behavior.
* Sensor Malfunction: While uncommon, the TDSDHT11 sensor itself might malfunction. If you've ruled out software and hardware issues, consider replacing the sensor.

**Additional Tips:**

* Error Messages: Pay close attention to any error messages displayed during compilation or execution. These messages often provide clues about the source of the problem.
* Online Resources: Utilize online forums, communities, and documentation related to the TDSDHT11 sensor and Arduino programming. Search for specific error codes or issues you encounter.
* Community Support: Engage with the Arduino community by seeking help from experienced users and developers. Share your code and describe the problems you're facing to get assistance.

**Happy Experimenting with TDSDHT11!**

* Remember that even after careful coding and debugging, there's a slight possibility of encountering bugs due to external factors or unforeseen circumstances. The key is to identify the source of the issue and apply appropriate solutions.
