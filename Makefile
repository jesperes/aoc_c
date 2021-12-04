
export CC=clang
export CFLAGS=-g -O3 -Wall -Wextra -Werror -MMD -Isrc -Isrc/2021 -D_GNU_SOURCE

BUILD_DIR=_build

default:
	$(MAKE) configure
	$(MAKE) compile
	$(MAKE) run

$(BUILD_DIR):
	mkdir -p $@

.PHONY: configure
configure: | $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..

.PHONY: compile
compile: | $(BUILD_DIR)
	cd $(BUILD_DIR) && $(MAKE) -s

.PHONY: run
run: | $(BUILD_DIR)
	cd $(BUILD_DIR) && ./aoc2021

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)