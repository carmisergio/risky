#pragma once

#include <stdint.h>

#include <results.h>

typedef enum {
    HASHMAP_OK = 0,
    HASHMAP_NOT_FOUND = 1,
    HASHMAP_MALFORMED = 2,
} hashmap_status;

typedef struct {
    uint64_t status;
    uint64_t value;
} hashmap_result_u64;

/*
 * Validate simple bytes->u64 hashmap
 * Checks the structure of the hashmap to make sure looking up any value does
 * not produce an invalid memory access
 *
 * Status: HASHMAP_OK when hashmap is valid, HASHMAP_MALFORMED otherwise
 */
hashmap_status simple_bytes_u64_hashmap_validate(uint8_t *hashmap,
                                                 uint64_t max);

/*
 * Look up value simple bytes->u64 hashmap
 *
 * Status: HASHMAP_OK when item found, HASHMAP_NOT_FOUND otherwise
 */
hashmap_result_u64 simple_bytes_u64_hashmap_lookup(uint8_t *hashmap,
                                                   uint8_t *key, uint64_t len);
