
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