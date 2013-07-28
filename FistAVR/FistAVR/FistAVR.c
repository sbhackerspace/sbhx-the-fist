/*
 * FistAVR.c
 *
 * Created: 7/24/2013 2:48:58
 *  Author: Ryan
 */ 

// F_CPU tells util/delay.h our clock frequency
#define F_CPU 16000000UL // Clock frequency (16MHz)

#include <avr/io.h>
#include <util/delay.h>

void delayms( uint16_t millis )
{
	while ( millis )
	{
		_delay_ms( 1 );
		millis--;
	}
}

int main(void)
{
	DDRB |= 1 << PB0;
	
	PORTB &= ~( 1 << PB0 );
	

	while ( 1 )
	{

		PORTB |= 1 << PB0;              // LED on

		delayms( 500 );                 // delay ms

		PORTB &= ~( 1 << PB0 );         // LED off

		delayms( 500 );                 // delay ms
		
	}
	
	return 0;
}


