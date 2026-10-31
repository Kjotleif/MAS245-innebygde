/*
 * Oppgave_7_6.cpp
 *
 * Created: 26/10/2026 20:14:40
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
	static uint8_t forrigeKnapTilstand = 1; 

	_delay_ms(20); // venter

	uint8_t knappensTilstand;

	if (PIND & (1 << PD1)) // PD1 er høy -> knappen er ikke trykket
	{
		knappensTilstand = 1; 
	}
	else				   // PD1 er lav -> knappen er trykket
	{
		knappensTilstand = 0; 
	}

	if ((forrigeKnapTilstand == 1) &&
	(knappensTilstand == 0)) // aktiveres kun når forrige og nåværing tilstander på knappen er ulike.
	{
		// LED blir PÅ hvis den var av, og blir AV hvis den var PÅ
		PORTD ^= (1 << PD0);
	}

	forrigeKnapTilstand = knappensTilstand; // tilstander blir lik igjen om vil bli lik så lenge knappen holdes
}

