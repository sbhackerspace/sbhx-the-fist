
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

ISR(PCINT0_vect)
{
	// ISR code to execute here
	//uint8_t led_pin = PB4;
	//encoder_value = 200;

	uint8_t button0 = 0;
	uint8_t button1 = 0;

	button0 = PINB & (1<<PB0);
	if (button0)
	{
		//PORTB |= 1 << led_pin;              // LED on
		encoder_value += 10;
	}
	
	button1 = PINB & (1<<PB1);
	if(button1)
	{
		//PORTB &= ~( 1 << led_pin );         // LED off
		encoder_value -= 10;
	}
}