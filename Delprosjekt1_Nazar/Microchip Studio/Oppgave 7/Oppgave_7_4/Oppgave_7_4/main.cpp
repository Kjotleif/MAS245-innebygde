/*
 * Oppgave_7_4.cpp
 *
 * Created: 25/10/2026 17:23:54
 * Author : nazar
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>


int main(void)
{
	
    PORTD |= (1 << PD0); // PD0 settes HØY -> lysdiode er AV
	DDRD |= (1 << PD0); // PD0 settes som utgang
	
	PORTD |= (1 << PD1); // Pull-up motstand aktiveres, PD1 nå leses HØY
	DDRD &= ~(1 << PD1); // PD1 settes som inngang
	
	
    while (1) 
    {
		if (!(PIND & (1 << PD1))){ // utføres når PD1 er LAV
			PORTD &= ~(1 << PD0); // PD0 settes LAV -> lysdiode er PÅ
		}
		else { // utføres når PD1 er HØY
			PORTD |= (1 << PD0); // PD0 settes HØY -> lysdiode er AV
		}
    }
}

