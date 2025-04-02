CC = mpicc
CFLAGS = -Wall -Wextra -std=c17 -pedantic

all: main

main: main.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) -fv main
