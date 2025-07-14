#include <Wire.h>
#include <SPI.h>
#include <SparkFun_AS7265X.h>

AS7265X sensor;
const int buttonPin = 2;  // Button pin to trigger sensor reading

void setup() {
  Serial.begin(9600);  // Start serial communication at 9600 baud
  Wire.begin();
  pinMode(buttonPin, INPUT);  // Using pull-down resistor for button

  // Initialize AS7265x sensor
  if (!sensor.begin()) {
    Serial.println("AS7265x not detected. Check wiring.");
    while (1);  // Halt if sensor is not found
  }
  // Serial.println("AS7265x initialized.");
}

void loop() {
  // Wait for button press (HIGH means pressed if using pull-down)
  if (digitalRead(buttonPin) == HIGH) {
    // Serial.println("Button pressed. Taking reading...");
    takeReadingAndSend();  // Take reading and send it to Python
    delay(1000);  // Debounce delay to avoid multiple readings per press
  }
}

void takeReadingAndSend() {
  sensor.takeMeasurementsWithBulb();  // Use the internal lamps

  // Get the 18 calibrated readings from the sensor
  float readings[18] = {
    sensor.getCalibratedA(), sensor.getCalibratedB(), sensor.getCalibratedC(),
    sensor.getCalibratedD(), sensor.getCalibratedE(), sensor.getCalibratedF(),
    sensor.getCalibratedG(), sensor.getCalibratedH(), sensor.getCalibratedI(),
    sensor.getCalibratedJ(), sensor.getCalibratedK(), sensor.getCalibratedL(),
    sensor.getCalibratedR(), sensor.getCalibratedS(), sensor.getCalibratedT(),
    sensor.getCalibratedU(), sensor.getCalibratedV(), sensor.getCalibratedW()
  };

  // Send readings over serial to Python (CSV format for easier parsing)
  for (int i = 0; i < 18; i++) {
    Serial.print(readings[i], 4);  // Print each reading with 4 decimal places
    if (i < 17) Serial.print(",");  // Add comma separator
    else Serial.println();  // End the line
  }
}
