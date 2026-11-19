/*
 * Oppgave_7_5.cpp
 *
 * Created: 26/10/2026 19:54:11
 * Author : nazar
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	 PORTD |= (1 << PD0); // PD0 settes HØY -> lysdiode er AV
	 DDRD |= (1 << PD0); // PD0 settes som utgang
	 
	 PORTD |= (1 << PD1); // Pull-up motstand aktiveres, PD1 nå leses HØY
	 DDRD &= ~(1 << PD1); // PD1 settes som inngang


	// Aktiver pin-change interrupt for port D
	PCICR |= (1 << PCIE2);

	// Aktiver interrupt for PD1
	PCMSK2 |= (1 << PCINT17);

	// Aktiver interrupts globalt
	sei();

	while (1)
	{
		// ingenting her...
		// all koden utføres av interrupt
	}
}


ISR(PCINT2_vect)
{
	if (!(PIND & (1 << PD1))){  // uføres kun når knappen er trykket, ikke når den er slippet
		
	// LED blir PÅ hvis den var av, og blir AV hvis den var PÅ
	PORTD ^= (1 << PD0); 
	
	}
}

