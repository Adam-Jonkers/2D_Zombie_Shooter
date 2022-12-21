# File:   Makefile
# Authors: Adam Jonkers
# Date:   2022-20-12
# Descr:  Makefile for game

# Definitions.
CC = gcc
CFLAGS = -Os -Wall -Wstrict-prototypes -Wextra -g 
DEL = rm


# Default target.
all: game.out


# Compile: create object files from C source files.
RPG_GAME.o: RPG_GAME.c MAP.h CHARACTERS.h CORE.h BATTLE.h
	$(CC) -c $(CFLAGS) $< -o $@

CHARACTERS.o: CHARACTERS.c CHARACTERS.h CORE.h
	$(CC) -c $(CFLAGS) $< -o $@

CORE.o: CORE.c CORE.h
	$(CC) -c $(CFLAGS) $< -o $@

BATTLE.o: BATTLE.c BATTLE.h CORE.h CHARACTERS.h
	$(CC) -c $(CFLAGS) $< -o $@

MAP.o: MAP.c MAP.h CORE.h CHARACTERS.h
	$(CC) -c $(CFLAGS) $< -o $@


# Link: create ELF output file from object files.
game.out: RPG_GAME.o CHARACTERS.o CORE.o BATTLE.o MAP.o
	$(CC) $(CFLAGS) $^ -o $@ -lm


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out







