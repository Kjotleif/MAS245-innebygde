/*
 * GccApplication1.cpp
 *
 * Created: 21.10.2026 10:35:53
 * Author : isakk
 */ 

#define F_CPU 100000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRD = (1<<PD0);
	int time = 0;
    /* Replace with your application code */
	PORTD |= (1<<PD0);
    while (1) 
    {
		PORTD ^= (1<<PD0);
		_delay_ms(1000);
    }
}

