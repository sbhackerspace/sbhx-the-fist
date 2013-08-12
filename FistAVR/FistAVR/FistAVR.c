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

#include "MotorDriver.h"

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
	Init_PWM0();
	Init_Motor1();
	
	
	DDRB |= 1 << PB0;
	
	PORTB &= ~( 1 << PB0 );
	
	Set_Motor1_Velocity( 100 );
	
	int16_t vel = 0;
	char dir = 1;

	while ( 1 )
	{

		//PORTB |= 1 << PB0;              // LED on

		//delayms( 500 );                 // delay ms

		//PORTB &= ~( 1 << PB0 );         // LED off

		//delayms( 500 );                 // delay ms
		
		delayms( 50 );                 // delay ms
		
		
		if (dir == 1)
		{
			vel += 10;
			Set_Motor1_Velocity(vel);
			if (vel >= 240)
			{
				dir = 0;
			}
		}
		else
		{
			vel -= 10;
			Set_Motor1_Velocity(vel);
			if (vel <= 20)
			{
				dir = 1;
			}
		}		
		
	}
	
	return 0;
}


