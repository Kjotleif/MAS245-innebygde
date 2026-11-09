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
 
  display.setCursor(0, 0);
  display.print("MAS - Gruppe 2");

  display.display();
}

void loop() {
}