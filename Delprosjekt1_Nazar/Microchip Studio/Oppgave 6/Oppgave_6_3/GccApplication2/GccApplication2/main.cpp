/*
 * GccApplication2.cpp
 *
 * Created: 17.11.2026 00:17:45
 * Author : isakk
 */ 

#include <avr/io.h>

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>


constexpr uint32_t valgtFrekvens = 200; // Her velges det en passende frekvens slik at LEDen ikke flimrer

constexpr double periode = 1000000.0 / valgtFrekvens;


void diyDelay(uint16_t us){ // Egen funksjon for å få variabel delay.
	for (int i = 0; i < us; i++){
	_delay_us(1);
	}
}


int main(void)
{
	PORTD |= (1 << PD0); // siden pin PD0 skal sinke skal den først settes HØY for at LEDen skal være av
	DDRD |= (1 << PD0);
	
while (1){ // u går opp og ned mellom 0 og 100 og styrer dutycycle til pwm signalet
	for (int u = 0; u<100; u++){
		for (int i = 0; i < static_cast<int>(valgtFrekvens)/100; i++){
			PORTD &= ~(1 << PD0); // LED paa
			diyDelay(periode*u/100);

			PORTD |= (1 << PD0);  // LED av
			diyDelay(periode*(100-u)/100);
		}
	}

	for (int u = 100; u>0; u--){
		for (int i = 0; i < static_cast<int>(valgtFrekvens)/100; i++){
			PORTD &= ~(1 << PD0); // LED paa
			diyDelay(periode*u/100);

			PORTD |= (1 << PD0);  // LED av
			diyDelay(periode*(100-u)/100);
		}
	}
}
}

