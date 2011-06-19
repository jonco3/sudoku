/*
 * test.c
 *
 * Test cases for sudoku
 */

// enusure asserts are always turned on
#ifndef DEBUG
#define DEBUG
#endif

// directly include sudoku solver
#include "sudoku.c"

bool valid_counts(int counts[grid_size + 1])
	{
	for (int i = 1 ; i <= grid_size ; ++i)
		{
		if (counts[i] != 1)
			return false;
		}
	return true;
	}

bool valid_solution(grid g)
	{
	int counts[grid_size + 1];
	for (int i = 0 ; i < grid_size ; ++i)
		{
		memset(counts, 0, sizeof(counts));
		FOR_ROW_INDEX(i, j)
			{
			digit d = g[j];
			assert(d >= 1 && d <= grid_size);
			++counts[d];
			}
		if (!valid_counts(counts))
			return false;
		}

	for (int i = 0 ; i < grid_size ; ++i)
		{
		memset(counts, 0, sizeof(counts));
		FOR_COL_INDEX(i, j)
			++counts[g[j]];
		if (!valid_counts(counts))
			return false;
		}

	for (int i = 0 ; i < grid_size ; ++i)
		{
		memset(counts, 0, sizeof(counts));
		FOR_SQUARE_INDEX(i / square_size, i % square_size, j)
			{
			for (int k = 0 ; k < 3 ; ++k)
				++counts[g[j + k]];
			}
		}
	return true;
	}

void run_tests()
	{
	printf("Running tests...\n");

	printf("  test i/o\n");
	grid g;
	char input[digit_count + 1];
	for (int i = 0 ; i < digit_count ; ++i)
		input[i] = ' ' + i;
	input[digit_count] = 0;
    for (int i = 0 ; i < digit_count ; ++i)
		{
		assert_equal(ERR_BAD_INPUT_CHAR, parse_input(input, g));
		if (input[i] < '1' || input[i] > '9')
			input[i] = '.';
		}
	assert_equal(ERR_NONE, parse_input(input, g));
	char output[digit_count + 1];
	format_output(g, output);
	assert_equal(0, strcmp(input, output));

	printf("  test coordinates\n");
	assert_equal(0, row_from_index(8));
	assert_equal(1, row_from_index(9));
	assert_equal(0, col_from_index(0));
	assert_equal(1, col_from_index(1));
	assert_equal(0, index_from_coords(0, 0));
	assert_equal(80, index_from_coords(8, 8));
	for (int i = 0 ; i < digit_count ; ++i)
		{
		assert_equal(i, index_from_coords(row_from_index(i),
										  col_from_index(i)));
		}

	printf("  test digit sets\n");
	digit_set d = 0;
	assert_equal(0, digit_set_size(d));
	for (int i = 1 ; i <= grid_size ; ++i)
		{
		d |= 1 << i;
		assert_equal(i, digit_set_size(d));
		}

	printf("  test calculating digit sets for row/column/square\n");
	memset(g, 0, sizeof(g));
	assert_equal(0, row_digits_used(g, 0));
	assert_equal(0, col_digits_used(g, 0));
	assert_equal(0, square_digits_used(g, 0, 0));

	for (int i = 0 ; i < grid_size ; ++i)
		g[index_from_coords(i, 0)] = i + 1;
	for (int i = 0 ; i < grid_size ; ++i)
		{
		assert_equal(1 << (i + 1), row_digits_used(g, i));
		assert_equal(i == 0 ? all_digits : 0, col_digits_used(g, i));

		int sc = 3 * (i % square_size);
		int sr = 3 * (i / square_size);
		int es = sc == 0 ? (14 << sr) : 0;
		assert_equal(es, square_digits_used(g, sr, sc));
		}

	memset(g, 0, sizeof(g));
	for (int i = 0 ; i < grid_size ; ++i)
		g[index_from_coords(0, i)] = i + 1;
	for (int i = 0 ; i < grid_size ; ++i)
		{
		assert_equal(i == 0 ? all_digits : 0, row_digits_used(g, i));
		assert_equal(1 << (i + 1), col_digits_used(g, i));

		int sc = 3 * (i % square_size);
		int sr = 3 * (i / square_size);
		int es = sr == 0 ? (14 << sc) : 0;
		assert_equal(es, square_digits_used(g, sr, sc));
		}

	printf("  test validator\n");
	const char* solution = "794582136268931745315476982689715324432869571157243869821657493943128657576394218";
	assert_equal(ERR_NONE, parse_input(solution, g));
	for (int i = 0 ; i < digit_count ; ++i)
		{
		digit d = g[i];
		digit x = d + 1;
		if (x > grid_size)
			x = 1;
		g[i] = x;
		assert(!valid_solution(g));

		int j = (i + 7) % digit_count;
		x = g[j];
		if (x != d)
			{
			g[j] = d;
			g[i] = x;
			assert(!valid_solution(g));
			}

		g[i] = d;
		g[j] = x;
		}
	assert(valid_solution(g));

	printf("  test solving sudoku\n");
	const char* i1 = ".94...13..............76..2.8..1.....32.........2...6.....5.4.......8..7..63.4..8";
	const char* i2 = "4.....8.5.3..........7......2.....6.....8.4......1.......6.3.7.5..2.....1.4......";
	const char* i3 = ".................................................................................";
	assert_equal(ERR_NONE, parse_and_solve(i1));
	assert_equal(ERR_NONE, parse_and_solve(i2));
	assert_equal(ERR_NONE, parse_and_solve(i3));

	printf("All tests passed\n");
	}

int main(int argc, char* argv[])
	{
	run_tests();
	return 0;
	}
