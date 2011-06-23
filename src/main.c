/*
 * main.c
 * 
 * Sudoku main program
 */

// directly include sudoku solver
#include "sudoku.c"

err run_solver()
	{
	const int buffer_size = digit_count + 3;  // data + extra to detect overrun + newline + zero
	char buffer[buffer_size];
	
	for (;;)
		{
		char* c = fgets(buffer, buffer_size, stdin);
		if (!c)
			return feof(stdin) ? ERR_NONE : ERR_READ_INPUT;
		if (strlen(buffer) != digit_count + 1)
			return ERR_BAD_LINE_LENGTH;
		assert_equal('\n', buffer[digit_count]);
		buffer[digit_count] = '\0';
		err r = parse_and_solve(buffer);
		if (r != ERR_NONE)
			return r;
		}
	return ERR_NONE;
	}

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
