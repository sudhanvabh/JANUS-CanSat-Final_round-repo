/*
Workflow of the code:

setup:
  creates an instance of the BMP280 Altitude sensor as Adafruit_BMP280
  Initializes the EMA with the first altitude reading

loop: 
  Reading the current altitude from the sensor
  Updating the EMA with the new reading
  calculating the difference between the current and previous EMA values
  Determining if the CanSat is ascending, descending, or stationary based on the altitude difference
  using ema formula to calculate current ema
  printing the current EMA altitude and CanSat status to the serial monitor

known issues:
  the readings[] array keeps getting appended which may cause a memory error. 
  Can be fixed by removing the older values
*/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

const float smoothingFactor = 0.2; // Smoothing factor for EMA (between 0 and 1)
float ema = 0;  // The Exponential Moving Average
float sensorValue;
int threshold = 5;  // Threshold for determining if the CanSat is ascending or descending
float previousEma = 0; // To keep track of the previous EMA value

// Create an instance of the BMP280
Adafruit_BMP280 bmp;

void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  sensorValue = bmp.readAltitude(1013.25); // Initializing EMA with the first sample point
  ema = sensorValue;
  previousEma = ema;
}

void loop() {
  // Read the Altitude sensor
  sensorValue = bmp.readAltitude(1013.25);
  
  ema = (smoothingFactor * sensorValue) + ((1 - smoothingFactor) * previousEma); // Updating EMA
  
  float altitudeDifference = ema - previousEma; // Calculate the altitude difference
  
  // Determine the CanSat's state
  if (abs(altitudeDifference) > threshold) {
    if (altitudeDifference < 0) {
      Serial.println("CanSat is Descending");
    } else {
      Serial.println("CanSat is Ascending");
    }
  } else {
    Serial.println("CanSat is Stationary");
  }
  
  // Output the result
  Serial.print("Current EMA Altitude: ");
  Serial.println(ema);

  // Update previous EMA
  previousEma = ema;
  
  delay(1000);  // Delay to match your sensor reading rate
}