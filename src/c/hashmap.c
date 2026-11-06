#include "hashmap.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "crypto.h"
#include "results.h"

typedef struct {
    uint32_t n_buckets;
    uint32_t _pad;
} simple_bytes_u64_hashmap_header;

typedef struct {
    uint32_t offset;
    uint32_t len;
    uint64_t value;
} simple_bytes_u64_hashmap_bucket;

typedef struct {
    uint32_t n_items;
    uint32_t n_buckets;
} bidir_bytes_u32_map_header;

typedef struct {
    uint64_t offset;
    uint32_t len;
    uint32_t idx;
} bidir_bytes_u32_map_bucket;

uint64_t simple_bytes_u64_hashmap_validate(uint8_t *hashmap, uint64_t max) {
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

result simple_bytes_u64_hashmap_lookup(uint8_t *hashmap, uint8_t *key,
                                       uint64_t len) {
    result res;
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

        if (buckets[bucket].len == 0)
            break;

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

uint64_t bidir_bytes_u32_map_validate(uint8_t *map, uint64_t max) {
    uint32_t n_buckets, n_items, i;
    uint64_t buckets_size, labels_array_size, max_arena_offset;
    bidir_bytes_u32_map_bucket *buckets;
    bidir_bytes_u32_map_header *header;
    uint32_t *labels;

    // Check if we have enough space for the header
    if (max < sizeof(bidir_bytes_u32_map_header))
        return HASHMAP_MALFORMED;

    // Read header
    header = (bidir_bytes_u32_map_header *)map;
    n_items = header->n_items;
    n_buckets = header->n_buckets;

    // Check if we have enough space for the buckets and labels array
    buckets_size = sizeof(bidir_bytes_u32_map_bucket) * n_buckets;
    labels_array_size = sizeof(uint32_t) * n_items;
    if (max <
        sizeof(bidir_bytes_u32_map_header) + buckets_size + labels_array_size)
        return HASHMAP_MALFORMED;

    // Compute pointers to bucket array, labels array, and key arena
    buckets =
        (bidir_bytes_u32_map_bucket *)(map +
                                       sizeof(bidir_bytes_u32_map_header));
    labels =
        (uint32_t *)(map + sizeof(bidir_bytes_u32_map_header) + buckets_size);

    // Check every offset in the key arena
    max_arena_offset = max - sizeof(bidir_bytes_u32_map_header) - buckets_size -
                       labels_array_size;
    for (i = 0; i < n_buckets; i++) {
        if (buckets[i].offset + buckets[i].len > max_arena_offset)
            return HASHMAP_MALFORMED;
    }

    // Check every label in the labels array
    for (i = 0; i < n_items; i++) {
        if (labels[i] >= n_buckets)
            return HASHMAP_MALFORMED;
    }

    return HASHMAP_OK;
}

result bidir_bytes_u32_map_lookup(uint8_t *map, uint8_t *key, uint64_t len) {
    result res;
    uint32_t n_buckets, n_items, first_bucket, bucket;
    uint32_t i;
    bidir_bytes_u32_map_bucket *buckets;
    uint8_t *arena, *key_ptr;
    bidir_bytes_u32_map_header *header;

    // Read header
    header = (bidir_bytes_u32_map_header *)map;
    n_items = header->n_items;
    n_buckets = header->n_buckets;

    // Compute pointers to bucket array, labels array, and key arena
    buckets =
        (bidir_bytes_u32_map_bucket *)(map +
                                       sizeof(bidir_bytes_u32_map_header));
    arena = map + sizeof(bidir_bytes_u32_map_header) +
            n_buckets * sizeof(bidir_bytes_u32_map_bucket) +
            n_items * sizeof(uint32_t);

    // Compute key hash and find bucket
    first_bucket = hash_sdbm(key, len) % n_buckets;

    // Linear probing
    for (i = 0; i < n_buckets; i++) {

        bucket = (first_bucket + i) % n_buckets;

        if (buckets[bucket].len == 0)
            break;

        // Check key
        if (buckets[bucket].len != 0 && buckets[bucket].len == len &&
            memcmp((arena + buckets[bucket].offset), key, len) == 0) {
            // Key found!
            res.status = HASHMAP_OK;
            res.value = buckets[bucket].idx;
            return res;
        }
    }

    res.status = HASHMAP_NOT_FOUND;
    return res;
}

bytestr_result bidir_bytes_u32_map_revlookup(uint8_t *map, uint64_t idx) {
    bytestr_result res;
    uint32_t n_buckets, n_items;
    uint32_t i;
    bidir_bytes_u32_map_bucket *buckets;
    bidir_bytes_u32_map_header *header;
    uint8_t *arena;
    uint32_t *labels;
    uint32_t bucket;

    // Read header
    header = (bidir_bytes_u32_map_header *)map;
    n_items = header->n_items;
    n_buckets = header->n_buckets;

    if (idx >= n_items) {
        // Out of bounds
        res.ptr = NULL;
        return res;
    }

    // Compute pointers to bucket array, labels array, and key arena
    buckets =
        (bidir_bytes_u32_map_bucket *)(map +
                                       sizeof(bidir_bytes_u32_map_header));
    labels = (uint32_t *)(map + sizeof(bidir_bytes_u32_map_header) +
                          sizeof(bidir_bytes_u32_map_bucket) * n_buckets);
    arena = map + sizeof(bidir_bytes_u32_map_header) +
            n_buckets * sizeof(bidir_bytes_u32_map_bucket) +
            n_items * sizeof(uint32_t);

    bucket = labels[idx];
    res.ptr = arena + buckets[bucket].offset;
    res.len = buckets[bucket].len;

    return res;
}
