
#include <avr/io.h>
#include <avr/interrupt.h>


#include "EncoderReader.h"

void Init_Encoders()
{
	
	PCICR |= 1<<PCIE0; // Enable the pin change interrupts
	PCMSK0 |= 1<<PCINT0 | 1<<PCINT1 | 1<<PCINT2 | 1<<PCINT3; // Enable specific pin change interrupts
	
	EICRA |= 1<<ISC00 | 1<<ISC10 | 1<<ISC20 | 1<<ISC30; // Configure the external interrupts to trigger on all edges
	EIMSK |= 1<<INT0 | 1<<INT1 | 1<<INT2 | 1<<INT3; // Enable the external interrupts
	
}

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

ISR(PCINT0_vect) // encoders 0A, 0B, 1A, 1B
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

ISR(INT0_vect) // encoder 2A
{
	// Read the encoder pins
	const uint8_t encoderPins = PIND;
	
	const uint8_t encoderMask2 = 0x03; //0000 0011
	
	const uint8_t encoderPins2 = encoderPins & encoderMask2;
	
	// If the values have changed, update the encoder counts and old pin values
	if (oldEncoderPins[2] != encoderPins2)
	{
		encoder_value[2] += read_encoder(encoderPins2, oldEncoderPins[2]);
		oldEncoderPins[2] = encoderPins2;
	}
		
}

ISR(INT1_vect) // encoder 2B
{
	// Read the encoder pins
	const uint8_t encoderPins = PIND;
	
	const uint8_t encoderMask2 = 0x03; //0000 0011
	
	const uint8_t encoderPins2 = encoderPins & encoderMask2;
	
	// If the values have changed, update the encoder counts and old pin values
	if (oldEncoderPins[2] != encoderPins2)
	{
		encoder_value[2] += read_encoder(encoderPins2, oldEncoderPins[2]);
		oldEncoderPins[2] = encoderPins2;
	}	
	

}

ISR(INT2_vect) // encoder 3A
{
	// Read the encoder pins
	const uint8_t encoderPins = PIND;
	
	const uint8_t encoderMask3 = 0x0C; //0000 1100
	
	const uint8_t encoderPins3 = (encoderPins & encoderMask3) >> 2; // moves bits to rightmost for the encoder function

	// If the values have changed, update the encoder counts and old pin values
	if (oldEncoderPins[3] != encoderPins3)
	{
		encoder_value[3] += read_encoder(encoderPins3, oldEncoderPins[3]);
		oldEncoderPins[3] = encoderPins3;
	}	
	
}

ISR(INT3_vect) // encoder 3B
{
	// Read the encoder pins
	const uint8_t encoderPins = PIND;
	
	const uint8_t encoderMask3 = 0x0C; //0000 1100
	
	const uint8_t encoderPins3 = (encoderPins & encoderMask3) >> 2; // moves bits to rightmost for the encoder function

	// If the values have changed, update the encoder counts and old pin values
	if (oldEncoderPins[3] != encoderPins3)
	{
		encoder_value[3] += read_encoder(encoderPins3, oldEncoderPins[3]);
		oldEncoderPins[3] = encoderPins3;
	}	
	
}