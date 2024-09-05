


#include <Wire.h> // I2C communication
#include <MS4525DO.h> // MS4525DO sensor communication
#include <Adafruit_BMP280.h> // BMP for measuring static pressure

const float rho = 1.225;  // Air density

MS4525DO pitotSensor; // Instance Creation for MS4535
Adafruit_BMP280 bmp; // Instance Creation for BMP280
 
void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  
  if (!pitotSensor.begin()) {
    Serial.println("Couldn't start MS4525DO sensor.");
    while (1);  // Error stop
  }

  if (!bmp.begin()) {
    Serial.println("Couldn't start BMP280 sensor.");
    while (1);  // Error stop
  }
  
}

void loop() {
  // Read the raw pressure data from the sensor
  float pressure = pres.pres_pa(); // Reading from MS4525DO in Pas
  float staticPressure = bmp.readPressure(); // Reading from BMP280 in Pas
  float pressureDifference = abs(pressure - staticPressure); // calculating absolute difference

  /* from https://www.grc.nasa.gov/www/k-12/BGP/pitot.html
       V = sqrt [2 * {pt - ps} / rho ]
       where pt and ps are reading from MS4525DO and BMP280 respectively
       rho is the air density
  */
  
  nozzleVelocity = sqrt(2 * pressureDifference / rho)

  Serial.print("Exhaust Velocity: ")
  Serial.println(nozzleVelocity) // Printing the exhaust velocity on Serial

  delay(1000); // Waiting for a second before taking another reading
}