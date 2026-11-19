/*
 * Oppgave_6_2.cpp
 *
 * Created: 24/10/2026 11:53:13
 * Author : nazar
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

constexpr uint32_t f_10Hz = 10;
constexpr uint32_t f_20Hz = 20;
constexpr uint32_t f_30Hz = 30;
constexpr uint32_t f_35Hz = 35;
constexpr uint32_t f_100Hz = 100;
constexpr uint32_t f_200Hz = 200;

constexpr uint32_t valgtFrekvens = f_200Hz; // Her velges det en passende frekvens slik at LEDen ikke flimrer

constexpr double periode = 1000000.0 / valgtFrekvens;

int main(void)
{
	PORTD |= (1 << PD0); // siden pin PD0 skal sinke skal den først settes HØY for at LEDen skal være av
	DDRD |= (1 << PD0);

	while (1)
	{
		
		// 0% styrke i ca. 1 sekund
		for (int i = 0; i < static_cast<int>(valgtFrekvens); i++){
			PORTD |= (1 << PD0); // LED av
			_delay_us(periode);
		}
		// 30% styrke i ca. 1 sekund
		for (int i = 0; i < static_cast<int>(valgtFrekvens); i++){
			PORTD &= ~(1 << PD0); // LED på
			_delay_us(periode*0.3);

			PORTD |= (1 << PD0);  // LED av
			_delay_us(periode*0.7);
		}
		// 50% styrke i ca. 1 sekund
		for (int i = 0; i < static_cast<int>(valgtFrekvens); i++){
			PORTD &= ~(1 << PD0); // LED på
			_delay_us(periode*0.5);

			PORTD |= (1 << PD0);  // LED av
			_delay_us(periode*0.5);	
		}
		// 100% styrke i ca. 1 sekund
		for (int i = 0; i < static_cast<int>(valgtFrekvens); i++){
			PORTD &= ~(1 << PD0); // LED på
			_delay_us(periode);	
		}
		
	}
}

