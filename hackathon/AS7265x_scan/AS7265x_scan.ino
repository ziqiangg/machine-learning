#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SparkFun_AS7265X.h>

AS7265X sensor;
const int chipSelect = 10;
const char *filename = "PS.csv";
const int buttonPin = 2;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(buttonPin, INPUT); // using pull-down resistor

  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }
  Serial.println("SD card initialized.");

  // Initialize AS7265x
  if (!sensor.begin()) {
    Serial.println("AS7265x not detected. Check wiring.");
    while (1);
  }
  Serial.println("AS7265x initialized.");

  // If file doesn't exist, create it and write the CSV header
  if (!SD.exists(filename)) {
    Serial.println("File does not exist. Creating new file with header.");
    File file = SD.open(filename, FILE_WRITE);
    if (file) {
      file.println("A,B,C,D,E,F,G,H,I,J,K,L,R,S,T,U,V,W"); // 18 calibrated channels
      file.close();
      Serial.println("Header written to PS.csv");
    } else {
      Serial.println("Failed to create PS.csv");
      while (1);
    }
  } else {
    Serial.println("PS.csv exists. Waiting for button press...");
  }
}

void loop() {
  // Wait for button press (HIGH means pressed if using pull-down)
  if (digitalRead(buttonPin) == HIGH) {
    Serial.println("Button pressed. Taking reading...");
    takeReadingAndStore();
    delay(1000); // debounce delay to avoid multiple readings per press
  }
}

void takeReadingAndStore() {
  sensor.takeMeasurementsWithBulb(); // Use the internal lamp

  float readings[18] = {
    sensor.getCalibratedA(), sensor.getCalibratedB(), sensor.getCalibratedC(),
    sensor.getCalibratedD(), sensor.getCalibratedE(), sensor.getCalibratedF(),
    sensor.getCalibratedG(), sensor.getCalibratedH(), sensor.getCalibratedI(),
    sensor.getCalibratedJ(), sensor.getCalibratedK(), sensor.getCalibratedL(),
    sensor.getCalibratedR(), sensor.getCalibratedS(), sensor.getCalibratedT(),
    sensor.getCalibratedU(), sensor.getCalibratedV(), sensor.getCalibratedW()
  };

  // Print to Serial
  Serial.println("Storing 18-channel reading to SD card:");
  for (int i = 0; i < 18; i++) {
    Serial.print(readings[i], 4);
    if (i < 17) Serial.print(", ");
    else Serial.println();
  }

  // Append to CSV file
  File file = SD.open(filename, FILE_WRITE);
  if (file) {
    for (int i = 0; i < 18; i++) {
      file.print(readings[i], 4);
      if (i < 17) file.print(",");
      else file.println();
    }
    file.close();
    Serial.println("Reading saved to PS.csv");
  } else {
    Serial.println("Error writing to PS.csv");
  }
}
