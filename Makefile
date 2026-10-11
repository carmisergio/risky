# Programs
CC := riscv64-linux-gnu-gcc
QEMU := qemu-riscv64
RM := rm

# Files
BUILD_DIR := build
SRC_DIR := src
TEST_DIR := test
VENDOR_DIR := vendor
TEST_PREFIX := test_
TEST_SRCS := $(wildcard $(TEST_DIR)/$(TEST_PREFIX)*.c)
TEST_BINS_C := $(patsubst $(TEST_DIR)/$(TEST_PREFIX)%.c, $(BUILD_DIR)/$(TEST_PREFIX)%_c, $(TEST_SRCS))
TEST_BINS_ASM := $(patsubst $(TEST_DIR)/$(TEST_PREFIX)%.c, $(BUILD_DIR)/$(TEST_PREFIX)%_asm, $(TEST_SRCS))
UNITY_OBJ := $(BUILD_DIR)/unity.o
UNITY_DIR := $(VENDOR_DIR)/unity
UNITY_SRC := $(UNITY_DIR)/unity.c
SRC_C_DIR := $(SRC_DIR)/c
SRC_ASM_DIR := $(SRC_DIR)/asm

# Compiler settings
CFLAGS := -march=rv64gc -mabi=lp64d -g
LDFLAGS := -static -I$(UNITY_DIR)
OUT_DIR := build

# Unity settings
UNITY_FLAGS := -D UNITY_EXCLUDE_FLOAT -D UNITY_EXCLUDE_DOUBLE -D UNITY_EXCLUDE_MATH_H


.PHONY: test-all test-c test-asm clean

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
$(UNITY_OBJ): $(UNITY_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(UNITY_FLAGS) -c $< -o $@

# Compile C modules
$(BUILD_DIR)/%_c.o: $(SRC_C_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

# Compile assembly modules
$(BUILD_DIR)/%_asm.o: $(SRC_ASM_DIR)/%.s | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@
	
# Compile test modules
$(BUILD_DIR)/$(TEST_PREFIX)%.o: $(TEST_DIR)/$(TEST_PREFIX)%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@
	
# Link C test binaries
$(BUILD_DIR)/$(TEST_PREFIX)%_asm: $(BUILD_DIR)/$(TEST_PREFIX)%.o $(BUILD_DIR)/%_asm.o $(UNITY_OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# Link assembly test binaries
$(BUILD_DIR)/$(TEST_PREFIX)%_c: $(BUILD_DIR)/$(TEST_PREFIX)%.o $(BUILD_DIR)/%_c.o $(UNITY_OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BUILD_DIR): 
	mkdir -p $(BUILD_DIR)

clean:
	$(RM) -rf $(BUILD_DIR)


