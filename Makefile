NUM_REPS=1
WARNINGS= -Wall -Werror -Wno-gnu-empty-initializer
DEFINES= -D_GNU_SOURCE -DNUM_REPS=$(NUM_REPS)
INCLUDES= -Iinclude -Isrc/utils
LIBS=-lm
MEMORY_SANITIZER=t

ifdef MEMORY_SANITIZER
CC=gcc
CFLAGS=-fsanitize=address -g -MD $(WARNINGS) $(DEFINES) $(INCLUDES)
LDFLAGS=-fsanitize=address
else
CC=clang
CFLAGS=-g -Ofast -march=native -MD $(WARNINGS) $(DEFINES) $(INCLUDES)
endif

# TODO parameterize this on year
YEAR=2021
BUILD_DIR=build

default:
	@echo "CC=$(CC)"
	@echo "CFLAGS=$(CFLAGS)"
	@echo "REPS=$(NUM_REPS)"
	$(MAKE) aoc$(YEAR)
	$(MAKE) cppcheck
	./aoc$(YEAR)

UTILS_SRC=$(wildcard src/utils/*.c)
UTILS_OBJ=$(UTILS_SRC:src/utils/%.c=build/%.o)

LIBS+=build/libutils.a

INPUTS=$(wildcard inputs/$(YEAR)/*.txt)
INPUTS_SRC=$(INPUTS:inputs/$(YEAR)/%.txt=build/%.c)
INPUTS_OBJ=$(INPUTS_SRC:build/%.c=build/%.o)

SOURCES=$(wildcard src/$(YEAR)/*.c)
OBJECTS=$(SOURCES:src/$(YEAR)/%.c=build/%.o)
DEPS=$(OBJECTS:%.o=%.d) $(UTILS_OBJ:%.o=%.d)

-include $(DEPS)

$(BUILD_DIR)=build

build/libutils.a: $(UTILS_OBJ)
	ar rcs $@ $^

aoc2021: $(OBJECTS) $(INPUTS_OBJ) $(LIBS) | $(BUILD_DIR)
	$(CC) $^ $(LIBS) -o $@ $(LDFLAGS)

$(BUILD_DIR):
	mkdir -p $@

$(UTILS_OBJ): build/%.o: src/utils/%.c Makefile | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTS): build/%.o: src/$(YEAR)/%.c Makefile | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(INPUTS_OBJ): build/%.o: build/%.c Makefile | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(INPUTS_SRC): build/%.c: inputs/$(YEAR)/%.txt Makefile | $(BUILD_DIR)
	cp $< build/ && (cd build && xxd -i $*.txt) > $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) aoc$(YEAR)

.PHONY: valgrind
valgrind:
	$(MAKE) aoc2021
	valgrind --max-stackframe=3000000 --leak-check=full ./aoc2021

CPPCHECK=$(shell bash -c "command -v cppcheck")

ifneq ($(CPPCHECK),)
$(BUILD_DIR)/cppcheck.stamp: $(INPUTS_SRC) $(SOURCES) $(UTILS_SRC) Makefile
	$(CPPCHECK) -q -j$(shell nproc) $(INPUTS_SRC) $(SOURCES) $(UTILS_SRC) && touch $@

cppcheck: $(BUILD_DIR)/cppcheck.stamp
else
.PHONY:
cppcheck:
	@echo "cppcheck not installed"
endif
