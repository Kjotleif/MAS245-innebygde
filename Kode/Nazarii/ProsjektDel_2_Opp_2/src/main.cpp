#include <Arduino.h>
#include <FlexCAN_T4.h>

// Oppretter CAN-objekt
FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> can0;

void setup(){
  // Starter seriell kommunikasjon med en hastighet på 115200 bit/s
  Serial.begin(115200);

  // CAN0-grensesnittet med hastigheten satt til 500 kbit/s
  can0.begin();
  can0.setBaudRate(500000);
}

void loop(){

}
