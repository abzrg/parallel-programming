CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -pthread -g

BUILDDIR = build
SRCS = $(wildcard *.c)
TARGETS = $(addprefix $(BUILDDIR)/,$(patsubst %.c,%,$(SRCS)))

all: $(TARGETS)

$(BUILDDIR):
	mkdir -p $@

$(BUILDDIR)/%: %.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(TARGETS)

.PHONY: all clean
