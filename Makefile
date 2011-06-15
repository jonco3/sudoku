# Makefile for sudoku

CC = gcc
LD = gcc
FLAGS = -Wall -Werror -std=gnu99
CFLAGS = $(FLAGS)
LFLAGS = $(FLAGS)
DEBFLAGS = -g -DDEBUG
RELFLAGS = -O3
PROFFLAGS = $(RELFLAGS) -pg

SRCS = \
	src/sudoku.c

RELOBJS = $(subst src,build/rel,$(SRCS:.c=.o))
RELDEPS = $(subst src,build/rel,$(SRCS:.c=.d))
DEBOBJS = $(subst src,build/deb,$(SRCS:.c=.o))
DEBDEPS = $(subst src,build/deb,$(SRCS:.c=.d))
PROFOBJS = $(subst src,build/prof,$(SRCS:.c=.o))
PROFDEPS = $(subst src,build/prof,$(SRCS:.c=.d))

sudoku: $(RELOBJS)
	$(LD) $(RELOBJS) $(RELFLAGS) $(LFLAGS) -o sudoku

sudoku-debug: $(DEBOBJS)
	$(LD) $(DEBOBJS) $(DEBFLAGS) $(LFLAGS) -o sudoku-debug

sudoku-prof: $(PROFOBJS)
	$(LD) $(PROFOBJS) $(PROFFLAGS) $(LFLAGS) -o sudoku-prof

.PHONY: all
all: sudoku sudoku-debug sudoku-prof

.PHONY: test
test: sudoku-debug
	gdb -batch -return-child-result -command=debug.commands --args ./sudoku-debug -t

.PHONY: clean
clean:
	rm -f build/*/* sudoku*

-include $(RELDEPS)
-include $(DEBDEPS)
-include $(PROFDEPS)

build/rel/%.o: src/%.c
	mkdir -p build/rel
	$(CC) -c -MMD $(RELFLAGS) $(CFLAGS) $< -o $@

build/deb/%.o: src/%.c
	mkdir -p build/deb
	$(CC) -c -MMD $(DEBFLAGS) $(CFLAGS) $< -o $@

build/prof/%.o: src/%.c
	mkdir -p build/prof
	$(CC) -c -MMD $(PROFFLAGS) $(CFLAGS) $< -o $@
