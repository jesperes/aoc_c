default: aoc

CC=clang
CFLAGS=-g -O3 -Wall -Wextra -Werror -MMD -Isrc -Isrc/2021 -D_GNU_SOURCE
LDFLAGS=

INPUTS=$(wildcard inputs/2021/*.txt)
INPUT_SRC=$(INPUTS:inputs/2021/%.txt=src/2021/%.c)

SOURCES=$(wildcard src/2021/*.c src/*.c) $(INPUT_SRC)
OBJECTS=$(SOURCES:%.c=%.o)
DEPFILES=$(SOURCES:%.c=%.d)

-include $(DEPFILES)

$(INPUT_SRC): src/2021/%.c: inputs/2021/%.txt
	xxd -i $< > $@

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

aoc2021: $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

.PHONY: aoc
aoc: aoc2021
	./aoc2021

clean:
	rm -rf $(OBJECTS) $(DEPFILES) $(INPUT_SRC) aoc2021
