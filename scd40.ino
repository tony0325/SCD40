#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SparkFun_SCD4x_Arduino_Library.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const char* ssid = "OnHub";
const char* password = "1122334455";
const char* googleScriptURL = "https://script.google.com/macros/s/AKfycbxY7Db6Yv6yMEPBcgiSxhRzKM-7cHiWWEVHwwhEPYY06W5B5E5WpiPqFO-yV00a71b3/exec";

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
SCD4x mySensor;

void setup() {
  Serial.begin(115200);
  Serial.println("\n");
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (!mySensor.begin()) {
    Serial.println("SCD4x not found");
    while (1);
  }

  mySensor.startPeriodicMeasurement();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting to WiFi...");
    display.display();
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  mySensor.readMeasurement();
  display.print("CO2:");
  display.print(mySensor.getCO2());
  display.println("ppm");

  display.print("T:");
  display.print(mySensor.getTemperature());
  display.println(" C");

  display.print("H:");
  display.print(mySensor.getHumidity());
  display.println("%");

  display.display();

  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    String url = String(googleScriptURL) + "?temperature=" + mySensor.getTemperature() +
                 "&humidity=" + mySensor.getHumidity() + 
                 "&co2=" + mySensor.getCO2();
    
    
    http.begin(url);

    int httpCode = http.GET();

    display.clearDisplay();
    display.setCursor(0, 0);

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Data sent successfully");
    } else {
      Serial.println("Error sending data");
      Serial.println(httpCode);
    }

    http.end();
  }

  delay(60000); // Update every 60 seconds
}
