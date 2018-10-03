#ifndef CHIP_EIGHT_H
#define CHIP_EIGHT_H

#include <stdio.h>
#include <stdlib.h>

#include "chip_8_state.h"

Chip8State* InitializeChip ( void );
int LoadProgram ( Chip8State* , const char* );
void EmulateChip ( Chip8State* );

Chip8State* InitializeChip ( void )
{
	Chip8State* state = malloc ( sizeof ( Chip8State ) );

	state->memory = malloc ( 4096 * sizeof ( uint8_t ) );	//	Allocate 4K RAM
	state->SP = 0xEA0;	//	Memory Starts
	state->PC = 0x200;	//	Program Starts

	printf ( "PC Address : %d | SP Address : %d\n", state->PC, state->SP );

	return state;
}

int LoadProgram ( Chip8State* state, const char* path )
{
	FILE* rom = NULL; 

	if ( !( rom = fopen ( path, "rb" ) ) )
	{
		printf ( "Cannot Open ROM" );
		return 0;
	}

	printf ( "Loading ROM : %s", path );

	fseek ( rom, 0, SEEK_END );
	size_t file_size = ftell ( rom );
	fseek ( rom, 0, SEEK_SET );

	unsigned char* buffer = malloc ( file_size );

	fread ( buffer, file_size, 1, rom );

	fclose ( rom );

	//	Load Program Code into memory at location 0x200	
	for ( int i = 0; i < file_size; i++ )
	{
		state->memory [ 0x200 + i ] = buffer [ i ];
	}

	return 1;
}

void EmulateChip ( Chip8State* state )
{
	uint16_t op_code = state->memory [ state->PC ] << 8 | state->memory [ state->PC + 1 ];

	int highnib = ( op_code & 0xF000 ) >> 12;	//	Determine OpCode via highest 4-bits of 2 Byte Instruction

	printf ( "OPCODE : %d\n", highnib );
	system ( "pause" );

	switch ( highnib )
	{
		case 0x0:	
			switch ( ( op_code & 0x000F ) )
			{
				case 0x0:
					//	Clear Screen
					break;
				case 0xE:
					//	Subroutine Return
					state->PC = state->memory [ state->SP ];
					state->SP -= 2;
					break;
			}
			break;
		case 0x1:
			state->PC = ( op_code & 0x0FFF );
			break;
		case 0x2:	
			state->SP += 2;
			state->memory [ state->SP ] = state->PC;
			state->PC = ( op_code & 0x0FFF );
			break;
		case 0x3:
			if ( state->V [ ( op_code & 0x0F00 ) >> 8 ] == ( op_code & 0x00FF ) ) state->PC += 2;
			break;
		case 0x4:
			if ( state->V [ ( op_code & 0x0F00 ) >> 8 ] != ( op_code & 0x00FF ) ) state->PC += 2;
			break;
		case 0x5:
			if ( state->V [ ( op_code & 0x0F00 ) >> 8 ] == state->V [ ( op_code & 0x00F0 ) >> 4 ] ) state->PC += 2;
			break;
		case 0x6:
			state->V [ ( op_code & 0x0F00 ) >> 8 ] = ( op_code & 0x00FF );
			state->PC += 2;
			break;
		case 0x7:
			state->V [ ( op_code & 0x0F00 ) >> 8 ] += ( op_code & 0x00FF );
			state->PC += 2;
			break;
		case 0x8:

			switch ( ( op_code & 0x000F ) )
			{
				case 0x0:
					state->V [ ( op_code & 0x0F00 ) >> 8 ] = state->V [ ( op_code & 0x00F0 ) >> 4 ];
					state->PC += 2;
					break;
				case 0x1:
					state->V [ ( op_code & 0x0F00 ) >> 8 ] |= state->V [ ( op_code & 0x00F0 ) >> 4 ];
					state->PC += 2;
					break;
				case 0x2:
					state->V [ ( op_code & 0x0F00 ) >> 8 ] &= state->V [ ( op_code & 0x00F0 ) >> 4 ];
					state->PC += 2;
					break;
				case 0x3:
					state->V [ ( op_code & 0x0F00 ) >> 8 ] ^= state->V [ ( op_code & 0x00F0 ) >> 4 ];
					state->PC += 2;
					break;
				case 0x4:
					if ( state->V [ ( op_code & 0x00F0 ) >> 4 ] > ( 0xFF - state->V [ ( op_code & 0x0F00 ) >> 8 ] ) ) 
						state->V [ 0xF ] = 1;
					else 
						state->V [ 0xF ] = 0;

					state->V [ ( op_code & 0x0F00 ) >> 8 ] += state->V [ ( op_code & 0x00F0 ) >> 4 ];
					state->PC += 2;
					break;
				case 0x5:
					if ( state->V [ ( op_code & 0x0F00 ) >> 8 ] > state->V [ ( op_code & 0x00F0 ) >> 4 ] )
						state->V [ 0xF ] = 1;
					else
						state->V [ 0xF ] = 0;

					state->V [ ( op_code & 0x0F00 ) >> 8 ] -= state->V [ ( op_code & 0x00F0 ) >> 4 ];
					state->PC += 2;
					break;
				case 0x6:
					if ( ( state->V [ ( op_code & 0x0F00 ) >> 8 ] & 0x10000000 ) == 1 )
						state->V [ 0xF ] = 1;
					else
						state->V [ 0xF ] = 0;

					state->V [ ( op_code & 0x0F00 ) >> 8 ] /= 2;
					state->PC += 2;
					break;
			}
			break;
		case 0xA:
			state->I = ( op_code & 0x0FFF );
			state->PC += 2;
			break;
		case 0xD:
			break;
		default:
			return;
	}
}
#endif
