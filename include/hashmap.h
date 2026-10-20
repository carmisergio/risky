#pragma once

#include <stdint.h>

#include <results.h>

uint64_result_t simple_bytes_u64_hashmap_lookup(uint8_t *hashmap, uint8_t *key,
                                                uint64_t len);
