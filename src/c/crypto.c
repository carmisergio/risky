#include "crypto.h"

/*
 * Compute SDBM hash of some bytes
 */
uint32_t hash_sdbm(uint8_t *data, uint64_t n) {
    uint32_t hash = 0;
    uint64_t i;

    for (i = 0; i < n; i++) {
        hash = data[i] + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}
