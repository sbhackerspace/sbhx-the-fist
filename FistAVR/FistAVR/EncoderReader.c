
#include <avr/io.h>
#include <avr/interrupt.h>


#include "EncoderReader.h"



int8_t read_encoder( uint8_t newEncoderPins, uint8_t oldEncoderPins)
{
	switch(oldEncoderPins) //Possible states: 00 01 11 10
	{
		case(0): // 00
		{
			if(newEncoderPins == (1<<0))			{ return 1; }//10->00
			if(newEncoderPins == (1<<1))			{ return -1; }//01->00
			break;
		}
		case((1<<1)): // 01
		{
			if(newEncoderPins == 0)					{ return 1; }//00->01
			if(newEncoderPins == ((1<<0)|(1<<1)))	{ return -1; }//11->01
			break;
		}
		case(((1<<0)|(1<<1))): // 11
		{
			if(newEncoderPins == (1<<1))			{ return 1; }//01->11
			if(newEncoderPins == (1<<0))			{ return -1; }//10->11
			break;
		}
		case((1<<0)): // 10
		{
			if(newEncoderPins == ((1<<0)|(1<<1)))	{ return 1; }//11->10
			if(newEncoderPins == 0)					{ return -1; }//00->10
			break;
		}
	}
	
	return 0;
}


volatile int16_t encoder_value0 = 0;
volatile uint8_t oldEncoderPins0 = -1; //this large value will cause the variable to be initialized only in the ISR


ISR(PCINT0_vect)
{
	// ISR code to execute here

	uint8_t newEncoderPins0 = PINB & (1<<PB0 | 1<<PB1); //mask pins to get 0 and 1 values
	
	if (oldEncoderPins0 != newEncoderPins0)
	{
		encoder_value0 += read_encoder(newEncoderPins0, oldEncoderPins0);
		oldEncoderPins0 = newEncoderPins0;
	}
	
		
	
	
}