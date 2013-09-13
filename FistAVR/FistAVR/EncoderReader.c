
#include <avr/io.h>
#include <avr/interrupt.h>


#include "EncoderReader.h"

// Global variables
volatile int16_t encoder_value[4];
volatile uint8_t oldEncoderPins[4];


void Init_Encoders()
{
	
	PCICR |= 1<<PCIE0; // Enable the pin change interrupts
	PCMSK0 |= 1<<PCINT0 | 1<<PCINT1 | 1<<PCINT2 | 1<<PCINT3 | 1<<PCINT4; // Enable specific pin change interrupts
	
	EICRA |= 1<<ISC10 | 1<<ISC20 | 1<<ISC30; // Configure the external interrupts to trigger on all edges
	EIMSK |= 1<<INT1 | 1<<INT2 | 1<<INT3; // Enable the external interrupts
	
	
	// Reset the encoder values to zero
	encoder_value[0] = 0;
	encoder_value[1] = 0;
	encoder_value[2] = 0;
	encoder_value[3] = 0;
		
	// These large values will cause the variables to be initialized in the ISR on the first run.
	oldEncoderPins[0] = -1;
	oldEncoderPins[1] = -1;
	oldEncoderPins[2] = -1;
	oldEncoderPins[3] = -1;
}

int8_t get_encoder_delta( uint8_t newEncoderPins, uint8_t oldEncoderPins)
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

void read_encoder(uint8_t encoderNumber, uint8_t rawEncoderPins, uint8_t encoderMask, uint8_t shiftAmount)
{
	const uint8_t encoderPins = (rawEncoderPins & encoderMask) >> shiftAmount; // mask and move bits to rightmost for the encoder function
	
	// If the values have changed, update the encoder counts and old pin values
	if (oldEncoderPins[encoderNumber] != encoderPins)
	{
		encoder_value[encoderNumber] += get_encoder_delta(encoderPins, oldEncoderPins[encoderNumber]);
		oldEncoderPins[encoderNumber] = encoderPins;
	}
}

void read_encoder2()
{
	const uint8_t rawEncoderPinsB = PINB; //pin B4
	const uint8_t rawEncoderPinsD = PIND; //pin D1
	
	uint8_t encoderPins = 0;
	encoderPins |= ((rawEncoderPinsB & 0x10) >> 4);
	encoderPins |= ((rawEncoderPinsD & 0x02) >> 0);
	
	if (oldEncoderPins[2] != encoderPins)
	{
		encoder_value[2] += get_encoder_delta(encoderPins, oldEncoderPins[2]);
		oldEncoderPins[2] = encoderPins;
	}
}

// Masks for encoders
#define encoderMask0 0x03
#define encoderMask1 0x0C
#define encoderMask2 0x03
#define encoderMask3 0x0C

// Amount to shift masked pins so they are the bottom two bits
#define shiftAmount0 0
#define shiftAmount1 2
#define shiftAmount2 0
#define shiftAmount3 2


ISR(PCINT0_vect) // encoders 0A, 0B, 1A, 1B
{
	// Read the encoder pins
	const uint8_t rawEncoderPins = PINB;
	
	read_encoder(0, rawEncoderPins, encoderMask0, shiftAmount0);
	read_encoder(1, rawEncoderPins, encoderMask1, shiftAmount1);
	read_encoder2();
}

ISR(INT1_vect) // encoder 2B
{
	// Read the encoder pins
	const uint8_t rawEncoderPins = PIND;
	
	read_encoder(2, rawEncoderPins, encoderMask2, shiftAmount2);
	read_encoder2();
}

ISR(INT2_vect) // encoder 3A
{
	// Read the encoder pins
	const uint8_t rawEncoderPins = PIND;
	
	read_encoder(3, rawEncoderPins, encoderMask3, shiftAmount3);
}

ISR(INT3_vect) // encoder 3B
{
	// Read the encoder pins
	const uint8_t rawEncoderPins = PIND;

	read_encoder(3, rawEncoderPins, encoderMask3, shiftAmount3);
}