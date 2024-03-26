# Makefile for Lex project

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

# Object files
OBJS = Lex.o List.o

# Executable name
EXE = Lex

# Default target
$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS)

# Object file for Lex.c
Lex.o: Lex.c List.h
	$(CC) $(CFLAGS) -c Lex.c

# Object file for List.c
List.o: List.c List.h
	$(CC) $(CFLAGS) -c List.c

# Clean
clean:
	rm -f $(EXE) $(OBJS)

# Phony targets
.PHONY: clean
