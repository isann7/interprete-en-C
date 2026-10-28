CC ?= cc
CPPFLAGS += -Isrc -Isrc/common -Isrc/frontend -Isrc/ast -Isrc/bytecode -Isrc/compiler -Isrc/runtime -Isrc/vm -Isrc/stdlib
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O0 -g
LDFLAGS ?=

BUILD_DIR := build
TARGET := $(BUILD_DIR)/izlang
SOURCES := $(shell find src -type f -name '*.c' -print | sort)
OBJECTS := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

.PHONY: all clean legacy

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Build the archived interpreter only when explicitly requested.
legacy:
	$(MAKE) -C legacy/treewalk

clean:
	rm -rf $(BUILD_DIR)
	rm -f legacy/treewalk/interprete
