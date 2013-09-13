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
	TCCR0A &= ~(1<<WGM01);
	TCCR0B &= ~(1<<WGM02);

	// Set the clock source of the timer to be
	// the I/O clock/8  prescaling
	// TODO: set this to clock/1. This will push the 
	//  frequency above audible range.
	TCCR0B &= ~(1<<CS00);
	TCCR0B |= (1<<CS01);
	TCCR0B &= ~(1<<CS02);

	// Zero both of the compare registers
	OCR0A = 0;
	OCR0B = 0;

	// Enable the PWM output
	TCCR0A &= ~(1<<COM0A0); // timer 0 output A
	TCCR0A |= (1<<COM0A1);
	
	TCCR0A &= ~(1<<COM0B0); // timer 0 output B
	TCCR0A |= (1<<COM0B1);

	// Set both the pwm pin and the direction pin to be outputs
	DDRB |=  (1<<7); // OC0A
	DDRD |=  (1<<0); // OC0B
	DDRB |=  (1<<5); // OC1A
	DDRB |=  (1<<6); // OC1B
	
}

void Set_Motor_Velocity(uint8_t motor_number, int16_t  velocity)
{
	
	// We only have 4 motors
	if (motor_number > 3)
	{
		return;
	}

	// Max velocity is 255, we are going to limit it to 254
	if(velocity > 254)
	velocity = 254;
	else if (velocity < -254)
	velocity = -254;

	if(velocity >= 0)
	{
		// Set the data direction bit
		//PORTD |= (1<<PD4);

		// Set the velocity
		OCR0A = velocity;
	}
	else
	{
		//set the data direction bit
		//PORTD &= ~(1<<PD4);

		// Make the velocity positive
		velocity = -velocity;

		// Set the velocity
		OCR0A = velocity;
	}
}
