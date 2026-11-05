#include "hashmap.h"

#include <string.h>

#include "crypto.h"

typedef struct {
    uint32_t n_buckets;
    uint32_t _pad;
} simple_bytes_u64_hashmap_header;

typedef struct {
    uint32_t offset;
    uint32_t len;
    uint64_t value;
} simple_bytes_u64_hashmap_bucket;

hashmap_status simple_bytes_u64_hashmap_validate(uint8_t *hashmap,
                                                 uint64_t max) {
    uint32_t n_buckets, i;
    uint64_t buckets_size, max_arena_offset;
    simple_bytes_u64_hashmap_bucket *buckets;

    // Check if we have enough space for the header
    if (max < sizeof(simple_bytes_u64_hashmap_header))
        return HASHMAP_MALFORMED;

    // Read header
    n_buckets = ((simple_bytes_u64_hashmap_header *)hashmap)->n_buckets;

    // Check if we have enough space for the buckets
    buckets_size = sizeof(simple_bytes_u64_hashmap_bucket) * n_buckets;
    if (max < sizeof(simple_bytes_u64_hashmap_header) +
                  sizeof(simple_bytes_u64_hashmap_bucket) * n_buckets)
        return HASHMAP_MALFORMED;

    // Compute pointers to bucket array and key arena
    buckets = (simple_bytes_u64_hashmap_bucket
                   *)(hashmap + sizeof(simple_bytes_u64_hashmap_header));

    // Check every offset in the key arena
    max_arena_offset =
        max - sizeof(simple_bytes_u64_hashmap_header) - buckets_size;
    for (i = 0; i < n_buckets; i++) {
        if (buckets[i].offset + buckets[i].len > max_arena_offset)
            return HASHMAP_MALFORMED;
    }

    return HASHMAP_OK;
}

hashmap_result_u64 simple_bytes_u64_hashmap_lookup(uint8_t *hashmap,
                                                   uint8_t *key, uint64_t len) {
    hashmap_result_u64 res;
    uint32_t n_buckets, first_bucket, bucket;
    uint32_t i;
    simple_bytes_u64_hashmap_bucket *buckets;
    uint8_t *arena, *key_ptr;

    // Read header
    n_buckets = ((simple_bytes_u64_hashmap_header *)hashmap)->n_buckets;

    // Compute pointers to bucket array and key arena
    buckets = (simple_bytes_u64_hashmap_bucket
                   *)(hashmap + sizeof(simple_bytes_u64_hashmap_header));
    arena = hashmap + sizeof(simple_bytes_u64_hashmap_header) +
            n_buckets * sizeof(simple_bytes_u64_hashmap_bucket);

    // Compute key hash and find bucket
    first_bucket = hash_sdbm(key, len) % n_buckets;

    // Linear probing
    for (i = 0; i < n_buckets; i++) {

        bucket = (first_bucket + i) % n_buckets;

        // Check key
        if (buckets[bucket].len != 0 && buckets[bucket].len == len &&
            memcmp((arena + buckets[bucket].offset), key, len) == 0) {
            // Key found!
            res.status = HASHMAP_OK;
            res.value = buckets[bucket].value;
            return res;
        }
    }

    res.status = HASHMAP_NOT_FOUND;
    return res;
}
