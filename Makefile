# File:   Makefile
# Authors: Adam Jonkers
# Date:   2022-20-12
# Descr:  Makefile for game

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g 
DEL = rm


# Default target.
all: game.out


# Compile: create object files from C source files.
RPG_GAME.o: RPG_GAME.c MAP.h CHARACHTERS.h CORE.h BATTLE.h
	$(CC) -c $(CFLAGS) $< -o $@

CHARACHTERS.o: CHARACHTERS.c CHARACHTERS.h
	$(CC) -c $(CFLAGS) $< -o $@

CORE.o: CORE.c CORE.h
	$(CC) -c $(CFLAGS) $< -o $@

BATTLE.o: BATTLE.c BATTLE.h
	$(CC) -c $(CFLAGS) $< -o $@

MAP.o: MAP.c MAP.h
	$(CC) -c $(CFLAGS) $< -o $@


# Link: create ELF output file from object files.
game.out: RPG_GAME.o CHARACHTERS.o CORE.o BATTLE.o MAP.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@\
	

# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex







