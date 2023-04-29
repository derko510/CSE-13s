#include "stats.h"

#include <stdint.h>
#include <stdio.h>

void batcher_sort(Stats *stats, uint32_t *A, uint32_t n) {
    reset(stats);
    if (n == 0) {
        return;
    }
    // calculate the number of bits required to represent n
    uint32_t t = 0;
    while (n >> t) // n.bit_length
        t++;
    uint32_t p = 1 << (t - 1);
    while (p > 0) {
        uint32_t q = 1 << (t - 1); // q = 2^(t-1)
        uint32_t r = 0;
        uint32_t d = p;
        while (d > 0) {
            for (uint32_t i = 0; i < n - d; i++) {
                if ((i & p) == r
                    && cmp(stats, A[i], A[i + d])
                           == 1) { // == -1 for reverse sort and == 1 for normal
                    swap(stats, &A[i], &A[i + d]);
                }
            }
            d = q - p;
            q >>= 1;
            r = p;
        }
        p >>= 1;
    }
}
