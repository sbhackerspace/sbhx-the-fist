
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
	
	uint8_t i;
	for (i=0; i<4; ++i)
	{
		// 0x03 is a binary mask that gets two bits 0000 00XX
		const uint8_t shiftBits = (i<<1); // i*2
		
		// create mask that will extract the two pins we want
		const uint8_t mask = ((0x03) << shiftBits);
		
		// Apply the mask
		const uint8_t maskedEncoderPins = (encoderPins & mask);
		
		// Shift the pins so they are the bottom two bits.
		const uint8_t newEncoderPins = (maskedEncoderPins >> shiftBits);
		
		// If the values have changed, update the encoder counts and old pin values
		if (oldEncoderPins[i] != newEncoderPins)
		{
			encoder_value[i] += read_encoder(newEncoderPins, oldEncoderPins[i]);
			oldEncoderPins[i] = newEncoderPins;
		}
	}
	
	
	
}