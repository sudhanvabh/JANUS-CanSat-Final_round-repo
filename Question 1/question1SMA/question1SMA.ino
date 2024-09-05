/*
Workflow of the code:

setup:
  Creates an instance of the BMP280 Altitude sensor as Adafruit_BMP280
  Collects the first samplePoints amount of data points to display the first average.

loop: 
  Removing previous data
  Appending new data
  Calculating average
  Calculating average Altitude difference using the previous average data value
  printing onto the serial monitor
known issues:
  the readings[] array keeps getting appended which may cause a memory error. 
  Can be fixed by removing the older values
*/



#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

const int samplePoints = 3;  // Number of sample readings for moving average
int readings[samplePoints];  // Array to store the readings
int time = 0;  // Index of the current reading
int total = 0;  // Running total
float average = 0;  // The moving average
float sensorValue;
int threshold = 5;
int currentAverage = 0;

// Create an instance of the BMP280
Adafruit_BMP280 bmp;

void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  // taking first samplePoints for calculating the first moving average
  for (int i = 1; i <= samplePoints; i = i + 1) {
    sensorValue = bmp.readAltitude(1013.25);
    time = time + 1;
    readings[time] = sensorValue;
    total = total + readings[time];
  }
}

void loop() {
  
  // Moving Average COde
  sensorValue = bmp.readAltitude(1013.25); // reading the Altitude sensor at compared to 1013.25 Pascals pressure (sea level)
  int oldTime = time - samplePoints + 1;  // defning the time frame from which the old data should be deleted
  total = total - readings[oldTime];  // deleting the older data
  time = time + 1; 
  readings[time] = sensorValue; // Setting current reading
  total = total + readings[time]; // adding new data
  average = total / samplePoints;   // calculating average


  // calculating altitude difference for the ascent or descent declaration
  int oldAverage = currentAverage;  
  currentAverage = average;

  float altitudeDifference = currentAverage - oldAverage;  

  if (abs(altitudeDifference) > threshold) { //threshold amount to be decided in meters
    if (altitudeDifference < 0) {
      Serial.println("CanSat is Descending");
    }
    if (altitudeDifference > 0) {
      Serial.println("CanSat is Ascending");
    }
  } else {
    Serial.println("CanSat is Stationary");
  }

  // Reading the current altitude

  // Outputing the result
  Serial.print("Current Average Altitude: ");
  Serial.println(average);

  delay(1000);  // Delay to match your sensor reading rate
}