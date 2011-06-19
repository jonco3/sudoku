/*
 * main.c
 * 
 * Sudoku main program
 */

// directly include sudoku solver
#include "sudoku.c"

int main(int argc, char* argv[])
	{
	if (argc != 1)
		die("usage: soduku [-t]");
	
	err r = run_solver();
	switch (r)
		{
		case ERR_NONE:
			break;
		case ERR_READ_INPUT:
			die("error reading from stdin");			
		case ERR_BAD_INPUT_CHAR:
			die("input digit out of range, expected 0-9 or '.'");
		case ERR_BAD_LINE_LENGTH:
			die("bad input, expected line length of %d characters", digit_count);
		default:
			assert(0);
		}
	
	return 0;
	}
