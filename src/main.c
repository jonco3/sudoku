/*
 * main.c
 * 
 * Sudoku main program
 */

// directly include sudoku solver
#include "sudoku.c"
#include "ctype.h"

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

err solve_puzzles_from_file(const char* filename)
	{
	const int buffer_size = 256;
	
	FILE* file = fopen(filename, "r");
	if (!file)
		die("Can't open file %s", filename);
	int line = 0;
	
	for (;;)
		{
		char buffer[buffer_size];
		char* c = fgets(buffer, buffer_size, file);
		if (!c)
			{
			if (feof(file))
				return ERR_NONE;
			else
				die("Error reading file %s", filename);
			}
		++line;

		// strip training whitespace
		for (int i = strlen(buffer) - 1 ; i >= 0 ; --i)
			{
			if (isspace((int)buffer[i]))
				buffer[i] = '\0';
			}

		// skip empty lines and comments
		if (buffer[0] == '\0' || buffer[0] == '#')
			continue;
		
		if (strlen(buffer) != digit_count)
			die("line %d: bad line length");

		err r = parse_and_solve(buffer);
		if (r != ERR_NONE)
			return r;
		}
	
	fclose(file);
	return ERR_NONE;
	}

int main(int argc, char* argv[])
	{
	err r = ERR_NONE;
	if (argc == 1)
		r = run_solver();
	else if (argc == 2)
		r = solve_puzzles_from_file(argv[1]);
	else
		die("usage: soduku [ FILE ]");
	
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
