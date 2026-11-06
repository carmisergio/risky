#pragma once

#include <stdint.h>

#include <results.h>

enum hasmap_status {
    HASHMAP_OK = 0,
    HASHMAP_NOT_FOUND = 1,
    HASHMAP_MALFORMED = 2,
};

typedef struct {
    uint8_t *ptr;
    uint64_t len;
} bytestr_result;

/*
 * Validate simple bytes->u64 hashmap
 * Checks the structure of the hashmap to make sure looking up any value does
 * not produce an invalid memory access
 *
 * Status: HASHMAP_OK when hashmap is valid, HASHMAP_MALFORMED otherwise
 */
uint64_t simple_bytes_u64_hashmap_validate(uint8_t *hashmap, uint64_t max);

/*
 * Look up value in simple bytes->u64 hashmap
 *
 * Status: HASHMAP_OK when item found, HASHMAP_NOT_FOUND otherwise
 */
result simple_bytes_u64_hashmap_lookup(uint8_t *hashmap, uint8_t *key,
                                       uint64_t len);
/*
 * Validate bytes<->u32 bidirectional map
 * Checks the structure of the map to make sure looking up any value does
 * not produce an invalid memory access
 *
 * Status: HASHMAP_OK when hashmap is valid, HASHMAP_MALFORMED otherwise
 */
uint64_t bidir_bytes_u32_map_validate(uint8_t *map, uint64_t max);

/*
 * Look up index in bytes<->u32 bidirectional map by key
 *
 * Status: HASHMAP_OK when item found, HASHMAP_NOT_FOUND otherwise
 * Value: index
 */
result bidir_bytes_u32_map_lookup(uint8_t *map, uint8_t *key, uint64_t len);

/*
 * Look up key in bytes<->u32 bidirectional map by index
 *
 * Return: ptr == NULL when index out of bounds
 */
bytestr_result bidir_bytes_u32_map_revlookup(uint8_t *map, uint64_t idx);
