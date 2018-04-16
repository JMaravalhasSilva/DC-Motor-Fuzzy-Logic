#include "Encoder.h"

int8_t counter_lut[32] = {
		//Direction = 1
		0, // 00 to 00
	   -1, // 00 to 01
	   +1, // 00 to 10
	   +2, // 00 to 11

	   +1, // 01 to 00
		0, // 01 to 01
	   +2, // 01 to 10
	   -1, // 01 to 11

	   -1, // 10 to 00
	   +2, // 10 to 01
		0, // 10 to 10
	   +1, // 10 to 11

	   +2, // 11 to 00
	   +1, // 11 to 01
	   -1, // 11 to 10
		0, // 11 to 11

		//Direction = 0
		0, // 00 to 00
	   -1, // 00 to 01
	   +1, // 00 to 10
	   -2, // 00 to 11

	   +1, // 01 to 00
		0, // 01 to 01
	   -2, // 01 to 10
	   -1, // 01 to 11

	   -1, // 10 to 00
	   -2, // 10 to 01
		0, // 10 to 10
	   +1, // 10 to 11

	   -2, // 11 to 00
	   +1, // 11 to 01
	   -1, // 11 to 10
		0, // 11 to 11
};

uint32_t counter = 0,  direction = 0, lut_index = 0;

void encoderInterrupt(void){

	lut_index |= DIGITAL_IO_GetInput(&ENC_A)<<1 | DIGITAL_IO_GetInput(&ENC_B);
	counter += counter_lut[lut_index];

	if (counter_lut[lut_index] != 0){
		direction = (counter_lut[lut_index] > 0) ? 1 : 0;
	}

	//Prepare for next iteration by shifting current state
	//bits to old state bits and also the direction bit
	lut_index = ((lut_index << 2) & 0b1100) | (direction<<4);
}

uint32_t getEncoderValue(void){
	__disable_irq();
	uint32_t encoder_value = counter;
	__enable_irq();
	return encoder_value;
}

uint8_t getDirection(void){
	__disable_irq();
	uint8_t direction_value = direction;
	__enable_irq();
	return direction_value;
}
