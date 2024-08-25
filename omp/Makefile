CC = gcc
CFLAGS = -Wall -Wextra -O3 -fopenmp
BUILDDIR = build
SRC = $(wildcard *.c)
TARGETS = $(patsubst %.c,${BUILDDIR}/%,${SRC})

all: ${TARGETS}

build:
	mkdir build

${BUILDDIR}/%: %.c | build
	${CC} ${CFLAGS} $<	-o $@

clean:
	-@find build -type f -executable -exec rm -v {} \;

.PHONY: all clean
