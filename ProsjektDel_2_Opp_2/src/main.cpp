#include <Arduino.h>
#include <FlexCAN_T4.h>

// Oppretter CAN-objekt
FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> can0;
// Oppretter melding-objekt. Mottatte meldinger blir lagret i den.
CAN_message_t receivedMessage;
// Oppretter melding-objekt. Den skal innholde meldinger vi vil sende.
CAN_message_t sentMessage;


unsigned long lastSentTime = 0; // Tiden når siste melding ble sendt.
const unsigned long messageSendInterval = 1000; // Tid programmet skal vente før den sender en ny melding

void setup(){
  // Starter seriell kommunikasjon med en hastighet på 9600 bit/s
  Serial.begin(9600);

  // CAN0-grensesnittet med hastigheten satt til 500 kbit/s
  can0.begin();
  can0.setBaudRate(500000);

  // Lager en melding
  sentMessage.id = 0x123; // Jeg valgte ID-en lik gruppenummer
  sentMessage.len = 8; // Lengden på melding blir 8 byte

  // Hver byte får en random HEX verdi
  sentMessage.buf[0] = 0xCA;
  sentMessage.buf[1] = 0xFE;
  sentMessage.buf[2] = 0xCA;
  sentMessage.buf[3] = 0xFE;
  sentMessage.buf[4] = 0xCA;
  sentMessage.buf[5] = 0xFE;
  sentMessage.buf[6] = 0xCA;
  sentMessage.buf[7] = 0xFE;
}

void loop(){
  // millis() - gir tiden fra øyeblikket Teensy startet. 
  // Koden kjører når millis() er større enn tiden (når siste meldingen ble sent + interval)
  if (millis() > lastSentTime + messageSendInterval){
      can0.write(sentMessage);
      lastSentTime = millis();
  }
  // Vi har delay for at koden skal sende meldinger med jevne mellomrom og ikke overbelaste CAN bus



  // Leser melding fra can0 og lagrer den i receivedMessage. 
  // Utfører dette kun når vi klarer å lese meldingen.
  if (can0.read(receivedMessage))
  {
    Serial.print("Mottatt ID: 0x");
    Serial.println(receivedMessage.id, HEX); // Skriver ut meldings-ID-en i HEX format

    Serial.print("Lengde: ");
    Serial.println(receivedMessage.len); // Skriver ut meldingslengden

    for (uint8_t i = 0; i < receivedMessage.len; i++)
      {
      Serial.print(receivedMessage.buf[i], HEX); // Skriver ut meldinengs data byte for byte
      Serial.print(" ");
      }
      Serial.println(); // Går til neste linje


      can0.write(receivedMessage);
      // Melding var returnert
  }
}
