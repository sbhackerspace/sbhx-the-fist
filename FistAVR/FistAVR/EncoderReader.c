
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

volatile int16_t encoder_value[4] = {0, 0, 0, 0};

//These large values will cause the variables to be initialized in the ISR on the first run.
volatile uint8_t oldEncoderPins[4] = {-1, -1, -1, -1};

ISR(PCINT0_vect)
{
	// Read the encoder pins
	const uint8_t encoderPins = PINB;
	
	const uint8_t encoderMask0 = 0x03; //0000 0011
	const uint8_t encoderMask1 = 0x0C; //0000 1100
		
	const uint8_t encoderPins0 = encoderPins & encoderMask0;
	const uint8_t encoderPins1 = (encoderPins & encoderMask1) >> 2; // moves bits to rightmost for the encoder function
	
	// If the values have changed, update the encoder counts and old pin values
	if (oldEncoderPins[0] != encoderPins0)
	{
		encoder_value[0] += read_encoder(encoderPins0, oldEncoderPins[0]);
		oldEncoderPins[0] = encoderPins0;
	}	
	
	if (oldEncoderPins[1] != encoderPins1)
	{
		encoder_value[1] += read_encoder(encoderPins1, oldEncoderPins[1]);
		oldEncoderPins[1] = encoderPins1;
	}
	
}