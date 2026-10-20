#include "results.h"
#include <hashmap.h>

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t n_buckets;
    uint32_t _pad;
} SimpleBytesU64HashMapHeader;

typedef struct {
    uint32_t offset;
    uint32_t len;
    uint64_t value;
} SimpleBytesU64HashMapBucket;

/*
 * Look up a key in a simple bytes->u64 hashmap
 */
uint64_result_t simple_bytes_u64_hashmap_lookup(uint8_t *hashmap, uint8_t *key,
                                                uint64_t len) {
    uint64_result_t res;
    res.status = 0;
    return res;
}
