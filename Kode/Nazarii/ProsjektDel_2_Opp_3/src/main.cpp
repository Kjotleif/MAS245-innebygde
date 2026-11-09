#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

// Bredden og høyden i piksler på OLED skjermen
constexpr uint8_t screenWidth{128}; 
constexpr uint8_t screenHeight{64};

// Her defineres pinner på Teensy som er koblet til OLED-en
constexpr uint8_t Dc = 6;     // Data/Command koblet til pinne nr. 6
constexpr uint8_t Reset = 5;  // RESET er koblet til pinne nr. 5
constexpr uint8_t Cs = 10;    // Chip Select koblet til pinne nr. 10

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
  display.print("Antall mottatt: 0");

  display.setCursor(10, 52);
  display.print("Mottok sist ID: 000");

  // Tegner en kurve under tekst
  for (int i = -20; i < 20; i++){
    display.drawPixel(i + 64, 20 - (i * i) / 85, WHITE);
  }

  display.display();
}

void loop() {
}