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
assert_compile(sizeof(digit) == 1);

typedef digit grid[digit_count];

typedef unsigned short int digit_set;
assert_compile(sizeof(digit_set) == 2);

const digit_set all_digits = (1 << (grid_size + 1)) - 2;

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

int index_from_coords(unsigned int row, unsigned int col)
	{
	assert(row < grid_size);
	assert(col < grid_size);
	return row * grid_size + col;
	}

#define FOR_ROW_INDEX(row, i)                        \
	const int row_start = index_from_coords(row, 0); \
	const int row_end = row_start + grid_size;       \
	for (int i = row_start ; i < row_end ; ++i)

digit_set row_digits_used(grid g, unsigned int row)
	{
	digit_set result = 0;
	FOR_ROW_INDEX(row, i)
		{
		digit d = g[i];
		assert(d <= grid_size);
		result |= 1 << d;
		}
	return result & ~1;
	}

#define FOR_COL_INDEX(col, i)                              \
	const int col_start = index_from_coords(0, col);       \
	const int col_end = col_start + digit_count;           \
	for (int i = col_start ; i < col_end ; i += grid_size)

digit_set col_digits_used(grid g, unsigned int col)
	{
	digit_set result = 0;
	FOR_COL_INDEX(col, i)
		{
		digit d = g[i];
		assert(d <= grid_size);
		result |= 1 << d;
		}
	return result & ~1;
	}

#define FOR_SQUARE_INDEX(row, col, i)                             \
	const int square_start =                                      \
		(row - row % square_size) * grid_size +                   \
		(col - col % square_size);                                \
	const int square_end = square_start + grid_size * 3;          \
	for (int i = square_start ; i < square_end ; i += grid_size)

digit_set square_digits_used(grid g, unsigned int row, unsigned int col)
	{
	digit_set result = 0;
	FOR_SQUARE_INDEX(row, col, i)
		result |= 1 << g[i] | 1 << g[i + 1] | 1 << g[i + 2];
	return result & ~1;
	}

int digit_set_size(digit_set s)
	{
	// todo: better way
	int result = 0;
	for (int i = 1 ; i <= grid_size ; ++i)
		{
		if (s & (1 << i))
			++result;
		}
	return result;
	}

bool solve(grid g)
	{
	int index = -1;
	digit_set possibles = 0;
	int set_size = 0;
	
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
			int s = digit_set_size(p);

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

	for (int i = 1 ; i <= grid_size ; ++i)
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

err parse_and_solve(const char* input)
	{
	grid g;
	err r = parse_input(input, g);
	if (r != ERR_NONE)
		return r;
	if (solve(g))
		{
		char buffer[digit_count + 1];
		format_output(g, buffer);
		printf("%s\n", buffer);
		}
	else
		printf("failed to solve input\n");
	return ERR_NONE;
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
		err r = parse_and_solve(buffer);
		if (r != ERR_NONE)
			return r;
		}
	return ERR_NONE;
	}
