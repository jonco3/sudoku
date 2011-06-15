/*
 * soduku.c
 */

#include "myassert.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

enum
	{
	square_size = 3,
	grid_size = 9,
	digit_count = grid_size * grid_size,
	};

typedef unsigned char digit;
typedef digit grid[digit_count];

typedef unsigned short int digit_set;
const digit_set all_digits = (1 << grid_size) - 1;

typedef enum
	{
	ERR_NONE = 0,
	ERR_READ_INPUT,
	ERR_BAD_INPUT_CHAR,
	ERR_BAD_LINE_LENGTH,
	} err;

int row_from_index(int i)
	{
	return i / grid_size;
	}

int col_from_index(int i)
	{
	return i % grid_size;
	}

digit_set row_digits_used(grid g, int row)
	{
	const int start = row * grid_size;
	const int end = start + grid_size;
	digit_set result = 0;
	for (int i = start ; i < end ; ++i)
		result |= 1 << g[i];
	return result;
	}

digit_set col_digits_used(grid g, int col)
	{
	const int start = col;
	const int end = start + digit_count;
	digit_set result = 0;
	for (int i = start ; i < end ; i += grid_size)
		result |= 1 << g[i];
	return result;
	}

digit_set square_digits_used(grid g, int row, int col)
	{
	row -= row % square_size;
	col -= col % square_size;
	const int start = row * grid_size + col;
	const int end = start + grid_size * 3;
	digit_set result = 0;
	for (int i = start ; i < end ; i += grid_size)
		result |= 1 << g[i] | 1 << g[i + 1] | 1 << g[i + 2];
	return result;
	}

int get_set_size(digit_set s)
	{
	// todo: better way
	int result = 0;
	for (int i = 0 ; i < digit_count ; ++i)
		{
		if (s & (1 << i))
			++result;
		}
	return result;
	}

bool solve(grid g)
	{
	int index = -1;
	digit_set possibles;
	int set_size;
	
	for (int i = 0 ; i < digit_count ; ++i)
		{
		digit d = g[i];
		if (d == 0)
			{
			int row = row_from_index(i);
			int col = col_from_index(i);
			digit_set p = all_digits &
				~(row_digits_used(g, row) |
				  col_digits_used(g, col) |
				  square_digits_used(g, row, col));
			int s = get_set_size(p);

			if (index == -1 || s < set_size)
				{
				index = i;
				possibles = p;
				set_size = s;
				if (s == 1)
					break;
				}
			}
		}

	if (index == -1)
		return true;

	for (int i = 0 ; i < digit_count ; ++i)
		{
		if (possibles & (1 << i))
			{
			g[index] = i;
			if (solve(g))
				return true;
			}
		}

	g[index] = 0;
	return false;
	}

err parse_input(const char* input, grid output)
	{
	assert_equal(digit_count, strlen(input));
	for (int i = 0 ; i < digit_count ; ++i)
		{
		char c = input[i];
		if (c == '.')
			output[i] = 0;
		else
			{
			unsigned int digit = c - '0';
			if (digit > 9)
				return ERR_BAD_INPUT_CHAR;
			output[i] = digit;
			}
		}
	return ERR_NONE;
	}

void format_output(grid input, char* output)
	{
	for (int i = 0 ; i < digit_count ; ++i)
		{
		digit b = input[i];
		assert(b <= 9);
		output[i] = b == 0 ? '.' : ('0' + b);
		}
	output[digit_count] = '\0';
	}

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
		
		grid g;
		parse_input(buffer, g);
		format_output(g, buffer);
		printf("%s\n", buffer);
		}
	return ERR_NONE;
	}

err run_tests()
	{
	grid g;
	char *input = "0123456789......................................................................z";
	assert_equal(ERR_BAD_INPUT_CHAR, parse_input(input, g));
	input[0] = '.';
	assert_equal(ERR_BAD_INPUT_CHAR, parse_input(input, g));
	input[80] = '.';
	assert_equal(ERR_NONE, parse_input(input, g));
	for (int i = 0 ; i < digit_count ; ++i)
		{
		digit e = i <= 9 ? i : 0;
		assert_equal(e, g[i]);
		}

	char output[digit_count + 1];
	format_output(g, output);
	assert_equal(0, strcmp(input, output));
	
	return ERR_NONE;
	}

int main(int argc, char* argv[])
	{
	err r;
	if (argc == 2 && strcmp(argv[1], "-t") == 0)
		r = run_tests();
	else if (argc == 1)
		r = run_solver();
	else
		die("usage: soduku [-t]");

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
		}
	
	return 0;
	}
