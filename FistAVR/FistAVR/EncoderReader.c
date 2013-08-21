
#include <avr/io.h>
#include <avr/interrupt.h>


#include "EncoderReader.h"



int read_encoder(void)
{
	//int button0 = 0;
	//int button1 = 0;
//
	//button0 = PINB & (1<<PB0);
	//button1 = PINB & (1<<PB1);
	//
	return 0;
	
	
}


volatile int16_t encoder_value = 0;
volatile uint8_t oldEncoderPins = -1; //this large value will cause the variable to be initialized only in the ISR


ISR(PCINT0_vect)
{
	// ISR code to execute here

	uint8_t newEncoderPins = PINB & (1<<PB0 | 1<<PB1); //mask pins to get 0 and 1 values
	
	switch(oldEncoderPins) { //Possible states: 00 01 11 10
		case(0): // 00
		if(newEncoderPins == (1<<0)) { ++encoder_value; }//10->00
		if(newEncoderPins == (1<<1)) { --encoder_value; }//01->00
		break;
		case((1<<1)): // 01
		if(newEncoderPins == 0) { ++encoder_value; }//00->01
		if(newEncoderPins == ((1<<0)|(1<<1))) { --encoder_value; }//11->01
		break;
		case(((1<<0)|(1<<1))): // 11
		if(newEncoderPins == (1<<1)) { ++encoder_value; }//01->11
		if(newEncoderPins == (1<<0)) { --encoder_value; }//10->11
		break;
		case((1<<0)): // 10
		if(newEncoderPins == ((1<<0)|(1<<1))) { ++encoder_value; }//11->10
		if(newEncoderPins == 0) { --encoder_value; }//00->10
		break;
	}
		
	oldEncoderPins = newEncoderPins;	
	
	
}