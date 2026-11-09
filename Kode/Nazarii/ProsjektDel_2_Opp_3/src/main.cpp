#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <FlexCAN_T4.h>

// Oppretter CAN-objekt
FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> can0;
// Oppretter melding-objekt. Mottatte meldinger blir lagret i den.
CAN_message_t receivedMessage;
// Oppretter melding-objekt. Den skal innholde meldinger vi vil sende.
CAN_message_t sentMessage;


unsigned long lastSentTime{0}; // Tiden når siste melding ble sendt.
const unsigned long messageSendInterval{1000}; // Tid programmet skal vente før den sender en ny melding

unsigned long lastDispUpdateTime{0}; // Tiden når display var oppdatert sist.
constexpr unsigned long dispUpdateInterval{100}; // OLED-skjermen oppdateringsintervalle er 100. millisekund

// Verdier som skal skrives ut på OLED skjermen
int messagesReceivedCounter{0};
int messagesReceivedID{0};

// Bredden og høyden i piksler på OLED skjermen
constexpr uint8_t screenWidth{128}; 
constexpr uint8_t screenHeight{64};

// Her defineres pinner på Teensy som er koblet til OLED-en
constexpr uint8_t Dc = 6;     // Data/Command koblet til pinne nr. 6
constexpr uint8_t Reset = 5;  // RESET er koblet til pinne nr. 5
constexpr uint8_t Cs = 10;    // Chip Select koblet til pinne nr. 10

// Her defineres pinner på Teensy som er koblet til joystick
constexpr uint8_t joystickUp = 22;
constexpr uint8_t joystickDown = 23;

// Oppretter et displayobjekt som får de allerede definerte parametrene:
Adafruit_SSD1306 display(
  screenWidth,
  screenHeight,
  &SPI,
  Dc,
  Reset,
  Cs
);

void setup() {

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

  // Initialiserer OLED-skjermen
  if (!display.begin(SSD1306_SWITCHCAPVCC))
  {
    while (true)
    {
      // Hvis skjermen kan ikke initialiseres vil ikke programmet kjøre vidrere
    }
  }

  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Aktiverer interne pull-up motstandene til joystick
  pinMode(joystickUp, INPUT_PULLUP);
  pinMode(joystickDown, INPUT_PULLUP);
}



void loop() {
  // millis() - gir tiden fra øyeblikket Teensy startet. 
  // Koden kjører når millis() er større enn tiden (når siste meldingen ble sent + interval)
  if (millis() - lastSentTime >= messageSendInterval){
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

      messagesReceivedCounter++;
      messagesReceivedID = receivedMessage.id;

      can0.write(receivedMessage);
      // Melding var returnert
  }

  // Joystick logikken
  if (digitalRead(joystickDown) == LOW)
  {
    Serial.print("Going down."); 
    Serial.println();   
  } 
  else if (digitalRead(joystickUp) == LOW)
  {
    Serial.print("Going up.");
    Serial.println();
  }

  // Skjermen skal oppdateres hver 100 millesekund
  if (millis() - lastDispUpdateTime >= dispUpdateInterval){
    lastDispUpdateTime = millis();

    display.clearDisplay();
  
    display.setCursor(27, 5);
    display.print("MAS - Gruppe 2");

    // Tegner en avrundet rektangel
    display.drawRoundRect(0,0,screenWidth,screenHeight,5,WHITE);

    display.setCursor(10, 25);
    display.print("CAN - statistikk");    // Printer en tekst på OLED-en

    // Tegner en striplete linje
    bool wb_toggle{true};
    int count{0};
    for (int i = 10; i < screenWidth-10; i++){
      if (wb_toggle == true){
        display.drawPixel(i,37, WHITE);
      } else {
        display.drawPixel(i,37, BLACK);
      }
      if (count == 5){
        count = 0;
        if (wb_toggle == false){
          wb_toggle = true;
        } else {
          wb_toggle = false;
        }
      }
      count++;
    }

    display.setCursor(10, 42);
    display.print("Antall mottatt:");
    display.setCursor(100, 42);
    display.print(messagesReceivedCounter);

    display.setCursor(10, 52);
    display.print("Mottok sist ID:");
    display.setCursor(100, 52);
    display.print(messagesReceivedID, HEX);

    // Tegner en kurve under tekst
    for (int i = -20; i < 20; i++){
      display.drawPixel(i + 64, 20 - (i * i) / 85, WHITE);
    }

    display.display();
  }
}