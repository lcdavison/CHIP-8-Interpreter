#ifndef CHIP_EIGHT_STATE_H
#define CHIP_EIGHT_STATE_H

#include <stdint.h>

typedef struct 
{
	uint8_t 	V [ 16 ];	//	Registers, 16 8-bit registers
	uint16_t	I;		//	Address Register
	uint16_t	SP;		//	Stack Pointer
	uint16_t 	PC;		//	Program Counter
	uint8_t		delay;		//	Delay Timer
	uint8_t		sound;		//	Sound Timer
	uint8_t*	memory;		//	RAM
	uint8_t*	display;	//	Screen Data
} Chip8State;

#endif
