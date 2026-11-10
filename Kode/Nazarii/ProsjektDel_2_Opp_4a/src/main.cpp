#include <Arduino.h>
#include <Wire.h>
#include <MPU9250_WE.h>

constexpr uint8_t imuAddress = 0x68; // Oppretter en const med IMU adresse i HEX

unsigned long lastImuDataUpdateTime{0}; // Tiden når IMU data var oppdatert sist.
const unsigned long imuDataUpdateInterval{500}; // Tiden koden skal vente før den oppdaterer IMU data

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
  if (millis() - lastImuDataUpdateTime >= imuDataUpdateInterval){
    lastImuDataUpdateTime = millis();

    xyzFloat acceleration = imu.getGValues();
    // IMU gir akselerasjon i g verdier. For å få m/s^2 må man gange med 9.81 
    float accelerationX = acceleration.x * 9.81;
    float accelerationY = acceleration.y * 9.81;
    float accelerationZ = acceleration.z * 9.81;


    Serial.print("Akselerasjon X: ");
    Serial.println(accelerationX);

    Serial.print("Akselerasjon Y: ");
    Serial.println(accelerationY);

    Serial.print("Akselerasjon Z: ");
    Serial.println(accelerationZ);
  }
}

