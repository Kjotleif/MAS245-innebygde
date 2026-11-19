/*
 * GccApplication1.cpp
 *
 * Created: 21.10.2026 10:35:53
 * Author : isakk
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRD = (1 << PD0);
	
	/*
	while (1)
	{
		PORTD |= (1 << PD0);
		_delay_us(10000000);
		
		PORTD &= ~(1 << PD0);
		_delay_us(10000000);
	}
	*/

	/*while (1)
	{
		for (int i = 0; i < 100; i++)
		{

			PORTD = (1 << PD0);

			for (int j = 0; j < 100 - i; j++)
			_delay_us(500);

			PORTD = 0;

			for (int j = 0; j < i; j++)
			_delay_us(500);
		}
		
		
		for (int i = 100; i > 0; i--)
		{

			PORTD = (1 << PD0);

			for (int j = 0; j < 100 - i; j++)
			_delay_us(500);

			PORTD = 0;

			for (int j = 0; j < i; j++)
			_delay_us(500);
		}
	}*/
}

