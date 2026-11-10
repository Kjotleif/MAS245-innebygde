#include <Arduino.h>
#include <Wire.h>
#include <MPU9250_WE.h>

constexpr uint8_t imuAddress = 0x68; // Oppretter en const med IMU adresse i HEX

// Oppretter et IMU-objekt som bruker I2C-grensesnittet Wire1 og IMU-adressen
MPU9250_WE imu(&Wire1, imuAddress);

void setup() {
  Wire1.begin();  // Starter grensesnittet Wire1

  // Sjekker om Teensy ser IMU sensor.
  if (!imu.init())
  {
    Serial.println("FEIL: Fant ikke MPU9250.");

    while (true)
    {
        delay(1000);
    }
  }
}

void loop() {
}

