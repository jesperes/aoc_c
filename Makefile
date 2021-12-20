WARNINGS= -Wall -Werror -pedantic -Wno-gnu-empty-initializer
DEFINES= -D_GNU_SOURCE -DNUM_REPS=1
INCLUDES= -Iinclude -Isrc/utils

CC=clang
CFLAGS=-g -Ofast -march=native -MD $(WARNINGS) $(DEFINES) $(INCLUDES)

.SILENT:

# TODO parameterize this on year
YEAR=2021
BUILD_DIR=build

default:
	$(MAKE) aoc$(YEAR)
	$(MAKE) cppcheck
	./aoc$(YEAR)

UTILS_SRC=$(wildcard src/utils/*.c)
UTILS_OBJ=$(UTILS_SRC:src/utils/%.c=build/%.o)

LIBS=build/libutils.a

INPUTS=$(wildcard inputs/$(YEAR)/*.txt)
INPUTS_SRC=$(INPUTS:inputs/$(YEAR)/%.txt=build/%.c)
INPUTS_OBJ=$(INPUTS_SRC:build/%.c=build/%.o)

SOURCES=$(wildcard src/$(YEAR)/*.c)
OBJECTS=$(SOURCES:src/$(YEAR)/%.c=build/%.o)
DEPS=$(OBJECTS:%.o=%.d) $(UTILS_OBJ:%.o=%.d)

-include $(DEPS)

$(BUILD_DIR)=build

build/libutils.a: $(UTILS_OBJ)
	echo "AR\t$^"
	ar rcs $@ $^

aoc2021: $(OBJECTS) $(INPUTS_OBJ) $(LIBS) | $(BUILD_DIR)
	echo "LD\t$^"
	$(CC) $^ $(LIBS) -o $@

$(BUILD_DIR):
	mkdir -p $@

$(UTILS_OBJ): build/%.o: src/utils/%.c | $(BUILD_DIR)
	echo "CC\t$<"
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTS): build/%.o: src/$(YEAR)/%.c | $(BUILD_DIR)
	echo "CC\t$<"
	$(CC) $(CFLAGS) -c $< -o $@

$(INPUTS_OBJ): build/%.o: build/%.c | $(BUILD_DIR)
	echo "CC\t$<"
	$(CC) $(CFLAGS) -c $< -o $@

$(INPUTS_SRC): build/%.c: inputs/$(YEAR)/%.txt | $(BUILD_DIR)
	echo "XXD\t$<"
	cp $< build/ && (cd build && xxd -i $*.txt) > $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) aoc$(YEAR)

.PHONY: valgrind
valgrind:
	$(MAKE) aoc2021
	valgrind --max-stackframe=3000000 --leak-check=full ./aoc2021

.PHONY: cppcheck
cppcheck:
	echo "CPPCHECK"
	cppcheck -q -j$(shell nproc) $(INPUTS_SRC) $(SOURCES) $(UTILS_SRC)
