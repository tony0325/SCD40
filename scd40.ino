#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SparkFun_SCD4x_Arduino_Library.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
SCD4x mySensor;

void setup() {
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  if (!mySensor.begin()) {
    display.println("SCD4x not found");
    display.display();
    while (1);
  }

  // Start continuous measurement
  mySensor.startPeriodicMeasurement();
}

void loop() {
  // Assuming the library handles data availability internally
  mySensor.readMeasurement();

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
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
  
  delay(2000); // Update every 2 seconds
}
