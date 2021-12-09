CC=clang
CFLAGS=-g -Ofast -march=native -MD -Wall -Wextra -Werror -pedantic -D_GNU_SOURCE -Iinclude -DNUM_REPS=100

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
DEPS=$(OBJECTS:%.o=%.d)

-include $(DEPS)

INCLUDES=include src


$(BUILD_DIR)=build

aoc2021: $(OBJECTS) $(INPUTS_OBJ) | $(BUILD_DIR)
	$(CC) $^ -o $@

$(BUILD_DIR):
	mkdir -p $@

$(OBJECTS): build/%.o: src/$(YEAR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(INPUTS_OBJ): build/%.o: build/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(INPUTS_SRC): build/%.c: inputs/$(YEAR)/%.txt | $(BUILD_DIR)
	cp $< build/ && (cd build && xxd -i $*.txt) > $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) aoc$(YEAR)

.PHONY: valgrind
valgrind:
	valgrind --max-stackframe=3000000 ./aoc2021
