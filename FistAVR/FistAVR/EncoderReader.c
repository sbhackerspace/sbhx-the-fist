
#include <avr/io.h>

#include "EncoderReader.h"


int read_encoder(void)
{
	int button0 = 0;
	int button1 = 0;

	button0 = PINB & (1<<PB0);
	button1 = PINB & (1<<PB1);
	
	return 0;
	
	
}