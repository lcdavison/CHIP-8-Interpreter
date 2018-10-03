#include <stdlib.h>
#include <stdio.h>

#include "chip_8.h"

int main ( int argc, char** argv )
{
	Chip8State* state = InitializeChip (  );

	if ( !( LoadProgram ( state, argv [ 1 ] ) ) )
		return -1;
	
	while ( 1 )
	{
		EmulateChip ( state );
	}

	return 0;
}
