#include <stdlib.h>
#include <avr/io.h>

#include "MotorDriver.h"


// Motor stuff


static volatile  int16_t 	pour_pwm = 160;


//This initializes timer 0 (8 bits) to phase correct PWM
void Init_PWM()
{
	// Set timer 0 to wave generation mode phase correct PWM
	TCCR0A |= (1<<WGM00);
	
	// Set timer 1
	TCCR1A |= (1<<WGM10); //phase correct 8-bit PWM
	
	// Set the clock source of the timer to be
	// the I/O clock/8  prescaling
	// TODO: set this to clock/1. This will push the 
	//  frequency above audible range.
	TCCR0B &= ~(1<<CS00);
	TCCR0B |= (1<<CS01);
	TCCR0B &= ~(1<<CS02);
	
	TCCR1B &= ~(1<<CS00);
	TCCR1B |= (1<<CS01);
	TCCR1B &= ~(1<<CS02);

	// Zero both of the compare registers
	OCR0A = 0;
	OCR0B = 0;
	OCR1A = 0;
	OCR1B = 0;

	// Enable the PWM output
	//Clear on up counting, set when down counting
	TCCR0A |= (1<<COM0A1); // timer 0 output A
	TCCR0A |= (1<<COM0B1); // timer 0 output B
	TCCR1A |= (1<<COM1A1); // timer 1 output A
	TCCR1A |= (1<<COM1B1); // timer 1 output B

	// Set both the pwm pin and the direction pin to be outputs
	DDRB |=  (1<<7); // OC0A
	DDRD |=  (1<<0); // OC0B
	DDRB |=  (1<<5); // OC1A
	DDRB |=  (1<<6); // OC1B
	
	DDRD |=  (1<<4); // Direction 0
	DDRD |=  (1<<5); // Direction 1
	DDRD |=  (1<<6); // Direction 2
	DDRD |=  (1<<7); // Direction 3
}

void Set_Motor_Velocity(uint8_t motor_number, int16_t  velocity)
{
	
	// We only have 4 motors
	if (motor_number > 3)
	{
		return;
	}


	uint8_t reversing = (velocity < 0);

	// make the velocity have a positive value
	if (reversing)
	{
		velocity = -velocity; 
	}

	// Max velocity is 255, we are going to limit it to 254
	if(velocity > 254)
	{
		velocity = 254;
	}
	
	switch (motor_number)
	{
		case 0:
		{
			// Set the data direction bit
			if (reversing)
			{
				PORTD |= (1<<4);
			} 
			else
			{
				PORTD &= ~(1<<4);
			}

			// Set the duty cycle
			OCR0A = velocity;
			break;
		}
		case 1:
		{
			// Set the data direction bit
			if (reversing)
			{
				PORTD |= (1<<5);
			}
			else
			{
				PORTD &= ~(1<<5);
			}

			// Set the duty cycle
			OCR0B = velocity;
			break;
		}
		case 2:
		{
			// Set the data direction bit
			if (reversing)
			{
				PORTD |= (1<<6);
			}
			else
			{
				PORTD &= ~(1<<6);
			}

			// Set the duty cycle
			OCR1AL = velocity;
			break;
		}
		case 3:
		{
			// Set the data direction bit
			if (reversing)
			{
				PORTD |= (1<<7);
			}
			else
			{
				PORTD &= ~(1<<7);
			}
			
			// Set the duty cycle
			OCR1BL = velocity;
			break;
		}
	}
}

