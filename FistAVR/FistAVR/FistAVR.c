/*
 * FistAVR.c
 *
 * Created: 7/24/2013 2:48:58
 *  Author: Ryan
 */ 

// F_CPU tells util/delay.h our clock frequency
#define F_CPU 16000000UL // Clock frequency (16MHz)

#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "EncoderReader.h"
#include "MotorDriver.h"

void delayms( uint16_t millis )
{
	while ( millis )
	{
		_delay_ms( 1 );
		millis--;
	}
}

extern volatile int16_t encoder_value[];


int main(void)
{
	Init_PWM();
	Init_Encoders();
    
	//PCICR |= 1<<PCIE0; // Enable the pin change interrupts
	//PCMSK0 |= 1<<PCINT0 | 1<<PCINT1; // Enable specific pin change interrupts

	sei(); // Enable Global Interrupts
	
    Set_Motor_Velocity(0,0);
    
	
    
	//Set_Motor1_Velocity(encoder_value);
    
	//Enable the test LED
	//DDRB |= 1 << PB4;
	//PORTB &= ~( 1 << PB0 );
	//uint8_t led_pin = PB4;
	
	//Set_Motor1_Velocity( 100 );
	
	int16_t vel = 0;
	char dir = 1;
	
	//uint8_t button0 = 0;
	//uint8_t button1 = 0;
//
	//button0 = PINB & (1<<PB0);
	//button1 = PINB & (1<<PB1);
//
	while ( 1 )
	{
		//button0 = PINB & (1<<PB0);
		//if (button0)
		//{
			//PORTB |= 1 << led_pin;              // LED on
		//}
		//
		//button1 = PINB & (1<<PB1);
		//if(button1)
		//{
			//PORTB &= ~( 1 << led_pin );         // LED off
		//}

		//PORTB |= 1 << led_pin;              // LED on

		//delayms( 500 );                 // delay ms

		//PORTB &= ~( 1 << led_pin );         // LED off

		//delayms( 500 );                 // delay ms
        
		
        //int i = 0;
        //int j = 0;
        //int k = 0;
        //
        //while(i < 200)
        //{
            //while(j < 200)
            //{
                //k += 1;
            //}            
        //}
        
		//Set_Motor1_Velocity(100);
		delayms( 50 );                 // delay ms
		
		if (encoder_value[2] != 0)
        {
            Set_Motor_Velocity(0, encoder_value[2]);
        }
        else
        {            
			if (dir == 1)
			{
				vel += 10;
				Set_Motor_Velocity(0, vel);
				if (vel >= 240)
				{
					dir = 0;
				}
			}
			else
			{
				vel -= 10;
				Set_Motor_Velocity(0, vel);
				if (vel <= 20)
				{
					dir = 1;
				}
			}
        }            		
		
	}
	
	return 0;
}



