CC=clang
CFLAGS=-g -O3 -Wall -Wextra -Werror -D_GNU_SOURCE -Iinclude

# TODO parameterize this on year
YEAR=2021
BUILD_DIR=build

default: aoc$(YEAR)
	./aoc$(YEAR)

INPUTS=$(wildcard inputs/$(YEAR)/*.txt)
INPUTS_SRC=$(INPUTS:inputs/$(YEAR)/%.txt=build/%.c)
INPUTS_OBJ=$(INPUTS_SRC:build/%.c=build/%.o)

SOURCES=$(wildcard src/$(YEAR)/*.c)
OBJECTS=$(SOURCES:src/$(YEAR)/%.c=build/%.o)

INCLUDES=include src

$(BUILD_DIR)=build

aoc2021: $(OBJECTS) $(INPUTS_OBJ) | $(BUILD_DIR)
	$(CC) $^ -o $@

$(BUILD_DIR):
	mkdir -p $@

$(OBJECTS): build/%.o: src/$(YEAR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(INPUTS_OBJ): build/%.o: build/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(INPUTS_SRC): build/%.c: inputs/$(YEAR)/%.txt | $(BUILD_DIR)
	cp $< build/ && (cd build && xxd -i $*.txt) > $@

clean:
	rm -rf $(BUILD_DIR) aoc$(YEAR)
