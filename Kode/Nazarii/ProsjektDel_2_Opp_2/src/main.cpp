#include <Arduino.h>
#include <FlexCAN_T4.h>

// Oppretter CAN-objekt
FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> can0;
// Oppretter melding-objekt. Mottat meldinger blir lagret i den.
CAN_message_t receivedMessage;

void setup(){
  // Starter seriell kommunikasjon med en hastighet på 115200 bit/s
  Serial.begin(115200);

  // CAN0-grensesnittet med hastigheten satt til 500 kbit/s
  can0.begin();
  can0.setBaudRate(500000);
}

void loop(){
  // Leser melding fra can0 og lagrer den i receivedMessage. 
  // Utfører dette kun når vi klarer å lese meldingen.
  if (can0.read(receivedMessage))
  {
    Serial.print("Mottat ID: 0x");
    Serial.println(receivedMessage.id, HEX); // Skriver ut meldings-ID-en i HEX format

    Serial.print("Lengde: ");
    Serial.println(receivedMessage.len); // Skriver ut meldingslengden

    for (uint8_t i = 0; i < receivedMessage.len; i++)
      {
      Serial.print(receivedMessage.buf[i], HEX); // Skriver ut meldinengs data byte for byte
      Serial.print(" ");
      }
      Serial.println(); // Går til neste linje
  }
}
