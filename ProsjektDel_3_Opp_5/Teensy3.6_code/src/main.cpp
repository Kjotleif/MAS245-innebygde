#include <Arduino.h>
#include <Wire.h>
#include <MPU9250_WE.h>
#include <FlexCAN_T4.h>
#include <cmath>

constexpr uint8_t imuAddress = 0x68; // Oppretter en const med IMU adresse i HEX form

// Oppretter et IMU-objekt som bruker I2C-grensesnittet Wire1 og IMU-adressen
MPU9250_WE imu(&Wire1, imuAddress);

// Oppretter CAN-objekt
FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> can0;

// Oppretter melding-objekt. Mottatte meldinger blir lagret i den.
CAN_message_t receivedMessage;
// Oppretter melding-objekt. Den skal innholde meldinger vi vil sende.
CAN_message_t sentMessage;

// ID til CAN meldinger
constexpr uint32_t rawAngleCanId = 0x100;       // Oppretter en const (i HEX form) med ID til meldingen som skal sendes 
constexpr uint32_t filteredAngleCanId = 0x101;  // Oppretter en const (i HEX form) med ID til meldingen som skal mottas

float rawAngle = 0.0f;
float filteredAngle = 0.0f;

unsigned long lastUpdateTime{0}; // Tiden når IMU data var oppdatert og meldingen ble sendt sist .
const unsigned long updateInterval{100}; // Tiden koden skal vente før den oppdaterer 



// FUNKSJONSDEKLARASJONER

// Rå vinkel gjøres om til tre byte: fortegn, to sifer før koma, to sifer etter koma
void encodeAngle(float angle, uint8_t *buffer);


// Tre byte (fortegn, to sifer før koma, to sifer etter koma) gjøres om tilbake til en vinkel
float decodeAngle(const uint8_t *buffer);


void setup()
{
  Wire1.begin();  // Starter grensesnittet Wire1

  // Starter seriell kommunikasjon med en hastighet på 9600 bit/s
  Serial.begin(9600);

  // Sjekker om Teensy ser IMU sensor.
  if (!imu.init())
  {
    Serial.println("FEIL: Fant ikke MPU9250.");

    while (true)
    {
        delay(1000);
    }
  }

  // CAN0-grensesnittet med hastigheten satt til 250 kbit/s
  can0.begin();
  can0.setBaudRate(250000);

  sentMessage.id = rawAngleCanId;
  sentMessage.len = 3;
}


void loop()
{
  // Leser IMU og sender vinkelen hvert 100. ms
  if (millis() - lastUpdateTime >= updateInterval){
    lastUpdateTime = millis();

    xyzFloat acceleration = imu.getGValues();

    
    rawAngle = atan2f(acceleration.y,acceleration.z) * 180.0f / PI;   // Finner rå vinkel omkring x-aksen 
    encodeAngle(rawAngle, sentMessage.buf);   // Vinkel verdi gjøres om til tre byte og lagres inn i sentMessage for å bli sendt gjennom can

    can0.write(sentMessage); // Sender melding gjennom CAN til RPi

    // Skriver rå vinkel i Serial
    Serial.print("Rå vinkel: ");
    Serial.print(rawAngle, 2);
    Serial.print(" deg | ");
  }

  // Leser svaret fra RPi
  if (can0.read(receivedMessage))
  {
    // Sjekker at vi har mottat den riktige meldingen
    if (receivedMessage.id == filteredAngleCanId && receivedMessage.len == 3) { 
      filteredAngle = decodeAngle(receivedMessage.buf);   // Dekoder 3 byte tilbake til en vinkel

      // Skriver filtrert vinkel i Serial
      Serial.print("Filtrert vinkel: ");
      Serial.print(filteredAngle, 2);
      Serial.println(" deg");
    }
  }
}




// FUNKSJONSDEFINISJONER

void encodeAngle(float angle, uint8_t *buffer)
{
    // Sjekker fortegn til vinkelen og lagrer byte nr. 1
    if (angle >= 0.0f) {
      buffer[0] = 1;
    }
    else {
      buffer[0] = 0;
    }

    float absValAngle = fabsf(angle); // Oppretter en variabel med absolut verdi av vinkelen

    
    uint16_t angle_x_100 = static_cast<uint16_t>(roundf(absValAngle * 100.0f));   // 12.34 -> 1234

    buffer[1] = static_cast<uint8_t>(angle_x_100 / 100);    // Lagrer byte nr. 2.   1234 / 100 = 12 
    buffer[2] = static_cast<uint8_t>(angle_x_100 % 100);    // Lagrer byte nr. 3.   1234 % 100 = 34 (Resten av 1234 / 100)
}

float decodeAngle(const uint8_t *buffer)
{
  // Legger sammen byte nr. 2 * 100 + byte nr. 3.   12*100 + 34 = 1234
  int16_t vinkel = (buffer[1]*100 + buffer[2]);

  // Hvis byte nr. 1 er 0 blir vinkel negativ
  if (buffer[0] == 0){
    vinkel = -vinkel;
  }

  return static_cast<float>(vinkel) / 100.0f; // Deler på 100.    1234 / 100 = 12.34
}