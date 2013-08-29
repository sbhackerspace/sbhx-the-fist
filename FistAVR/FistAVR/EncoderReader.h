#ifndef ENCODER_READER_H_
#define ENCODER_READER_H_

void Init_Encoders();
	

int8_t read_encoder(uint8_t newEncoderPins, uint8_t oldEncoderPins);

//extern volatile int16_t encoder_value;


#endif // ENCODER_READER_H_
