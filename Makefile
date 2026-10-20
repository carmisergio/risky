# Programs
CC := riscv64-linux-gnu-gcc
QEMU := qemu-riscv64
RM := rm

# Directories
BUILD_DIR := build
SRC_DIR := src
INCLUDE_DIR := include
TEST_DIR := test
VENDOR_DIR := vendor
BIN_DIR := bin

# Compiler settings
CFLAGS := -march=rv64gc -mabi=lp64d -g -I$(INCLUDE_DIR)
CFLAGS_TEST := $(CFLAGS) -I$(VENDOR_DIR)/unity
LDFLAGS := -static

# Unity settings
UNITY_FLAGS := -D UNITY_EXCLUDE_FLOAT -D UNITY_EXCLUDE_DOUBLE -D UNITY_EXCLUDE_MATH_H

# Source discovery
C_SRCS := $(wildcard $(SRC_DIR)/c/*.c)
C_OBJS := $(patsubst $(SRC_DIR)/c/%.c, $(BUILD_DIR)/objs/c/%.o, $(C_SRCS))
ASM_SRCS := $(wildcard $(SRC_DIR)/asm/*.s)
ASM_OBJS := $(patsubst $(SRC_DIR)/c/%.c, $(BUILD_DIR)/objs/asm/%.o, $(ASM_SRCS))
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_BINS_C := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/test/c/%, $(TEST_SRCS))
TEST_BINS_ASM := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/test/asm/%, $(TEST_SRCS))
BINS_SRCS_C := $(wildcard $(BIN_DIR)/*.c)
BINS_SRCS_ASM := $(wildcard $(BIN_DIR)/*.s)
BINS := $(patsubst $(BIN_DIR)/%.c, $(BUILD_DIR)/bin/%, $(BINS_SRCS_C)) $(patsubst $(BIN_DIR)/%.s, $(BUILD_DIR)/bin/%, $(BINS_SRCS_ASM))

.PHONY: test-all test-c test-asm clean bins

bins: $(BINS)

test-all: test-c test-asm

# Run C tests
test-c: $(TEST_BINS_C)
	@for bin in $(TEST_BINS_C); do \
		$(QEMU) ./$$bin || exit 1; \
	done

# Run assembly tests
test-asm: $(TEST_BINS_ASM)
	@for bin in $(TEST_BINS_ASM); do \
		$(QEMU) ./$$bin || exit 1; \
	done

# Compile unity
$(BUILD_DIR)/objs/unity.o: $(VENDOR_DIR)/unity/unity.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_TEST) $(LDFLAGS) $(UNITY_FLAGS) -c $< -o $@

# Compile C modules
$(BUILD_DIR)/objs/c/%.o: $(SRC_DIR)/c/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

# Compile assembly modules
$(BUILD_DIR)/objs/asm/%.o: $(SRC_DIR)/asm/%.s
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@
	
# Compile test modules
$(BUILD_DIR)/objs/test/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_TEST) $(LDFLAGS) -c $< -o $@
	
# Link C test binaries
$(BUILD_DIR)/test/c/%: $(BUILD_DIR)/objs/test/%.o $(C_OBJS) $(BUILD_DIR)/objs/unity.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_TEST) $(LDFLAGS) $^ -o $@

# Link assembly test binaries
$(BUILD_DIR)/test/asm/%: $(BUILD_DIR)/objs/test/%.o $(ASM_OBJS) $(BUILD_DIR)/objs/unity.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_TEST) $(LDFLAGS) $^ -o $@

# Compile C binaries
$(BUILD_DIR)/objs/bin/%.o: $(BIN_DIR)/%.c 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@
	
# Compile asm binaries
$(BUILD_DIR)/objs/bin/%.o: $(BIN_DIR)/%.s
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

# Link binaries
$(BUILD_DIR)/bin/%: $(BUILD_DIR)/objs/bin/%.o $(C_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	$(RM) -rf $(BUILD_DIR)

