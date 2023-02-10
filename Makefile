# File:   Makefile
# Authors: Adam Jonkers
# Date:   2022-20-12
# Descr:  Makefile for game

# Definitions.
CC = gcc
CFLAGS = -Os -Wall -Wstrict-prototypes -Wextra -g -D_GNU_SOURCE -lmcheck
LINKS = -lm -lSDL2 -lSDL2_image -lpng -lSDL2_ttf -lSDL2_mixer
DEL = rm


# Default target.
all: game.out


# Compile: create object files from C source files.
RPG_GAME.o: RPG_GAME.c RPG_GAME.h MAP.h CORE.h ENTITIES.h
	$(CC) -c $(CFLAGS) $< $(LINKS) -o $@

CORE.o: CORE.c CORE.h
	$(CC) -c $(CFLAGS) $< $(LINKS) -o $@

MAP.o: MAP.c MAP.h CORE.h ENTITIES.h
	$(CC) -c $(CFLAGS) $< $(LINKS) -o $@

ENTITIES.o: ENTITIES.c ENTITIES.h CORE.h MAP.h
	$(CC) -c $(CFLAGS) $< $(LINKS) -o $@


# Link: create ELF output file from object files.
game.out: RPG_GAME.o CORE.o MAP.o ENTITIES.o
	$(CC) $(CFLAGS) $^ $(LINKS) -o $@


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out

# Target: run game.
.PHONY: play
play: game.out
	./game.out

# Target: debug game.
.PHONY: debug
debug: game.out
	gdb ./game.out

# Target: debug memory game.
.PHONY: mem
mem: game.out
	valgrind ./game.out







