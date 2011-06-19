# Makefile for sudoku

CC = gcc
LD = gcc
FLAGS = -Wall -Werror -std=gnu99
DEBFLAGS = $(FLAGS) -g -DDEBUG
RELFLAGS = $(FLAGS) -O3
PROFFLAGS = $(RELFLAGS) -pg

SRC = src/sudoku.c
DEP = src/myassert.h

sudoku: $(SRC) $(DEP)
	$(CC) $(SRC) $(RELFLAGS) -o sudoku

test-sudoku: $(SRC)
	$(CC) $(SRC) $(DEBFLAGS) -o test-sudoku

prof-sudoku: $(SRC)
	$(CC) $(SRC) $(PROFFLAGS) -o prof-sudoku

.PHONY: all
all: sudoku test-sudoku prof-sudoku

.PHONY: test
test: test-sudoku
	gdb -batch -command=debug.commands --args ./test-sudoku -t

.PHONY: clean
clean:
	rm -f sudoku test-sudoku prof-sudoku
