# Makefile for sudoku

CC = gcc
LD = gcc
FLAGS = -Wall -Werror -std=gnu99
DEBFLAGS = $(FLAGS) -g -DDEBUG
RELFLAGS = $(FLAGS) -O3
PROFFLAGS = $(RELFLAGS) -pg

MAIN_SRC = src/main.c
TEST_SRC = src/test.c
DEPS = src/myassert.h

sudoku: $(MAIN_SRC) $(DEPS)
	$(CC) $(MAIN_SRC) $(RELFLAGS) -o sudoku

prof-sudoku: $(MAIN_SRC) $(DEPS)
	$(CC) $(MAIN_SRC) $(PROFFLAGS) -o prof-sudoku

test-sudoku: $(TEST_SRC) $(DEPS)
	$(CC) $(TEST_SRC) $(DEBFLAGS) -o test-sudoku

.PHONY: all
all: sudoku test-sudoku prof-sudoku

.PHONY: test
test: test-sudoku
	gdb -batch -command=debug.commands --args ./test-sudoku

.PHONY: clean
clean:
	rm -f sudoku test-sudoku prof-sudoku
