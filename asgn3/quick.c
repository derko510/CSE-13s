#include "stats.h"

#include <stdint.h>
#include <stdio.h>

uint32_t partition(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    uint32_t i
        = lo
          - 1; // i is used to keep track of the boundary between elements smaller and greater than pivot
    for (uint32_t j = lo; j < hi; j++) { // loop through the sub-array to partition the elements
        // == -1 normal // == 1 reverse order
        if (cmp(stats, A[j - 1], A[hi - 1])
            == -1) { // check if the current element is smaller than the pivot
            i++;
            swap(stats, &A[i - 1], &A[j - 1]);
        }
    }
    swap(stats, &A[i], &A[hi - 1]);
    return i + 1; // return the index of the pivot
}

void quick_sorter(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    if (lo < hi) { // check if the sub-array has more than one element
        uint32_t p
            = partition(stats, A, lo, hi); // partition the sub-array and get the index of the pivot
        quick_sorter(stats, A, lo, p - 1); // sort the sub-array on the left side of the pivot
        quick_sorter(stats, A, p + 1, hi); // sort the sub-array on the right side of the pivot
    }
}

void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    reset(stats);
    quick_sorter(stats, A, 1, n);
}
