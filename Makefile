default: aoc

CC=gcc
CFLAGS=-g -O3 -Wall -Wextra -Werror -MMD -Isrc -Isrc/2021 -D_GNU_SOURCE
LDFLAGS=

SOURCES=$(wildcard src/2021/*.c src/*.c)
OBJECTS=$(SOURCES:%.c=%.o)
DEPFILES=$(SOURCES:%.c=%.d)

-include $(DEPFILES)

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

aoc2021: $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

unittests:

.PHONY: aoc
aoc: aoc2021
	./aoc2021

clean:
	rm -rf $(OBJECTS) $(DEPFILES) aoc2021
