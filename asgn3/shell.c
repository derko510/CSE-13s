#include "gaps.h"
#include "stats.h"

#include <stdint.h>
#include <stdio.h>

void shell_sort(Stats *stats, uint32_t *arr, uint32_t length) {
    reset(stats);
    for (uint32_t gap = 0; gap < GAPS; gap++) {
        for (uint32_t i = gaps[gap]; i < length; i++) {
            uint32_t j = move(stats, i);
            uint32_t temp = arr[i];
            while (
                j >= gaps[gap]
                && cmp(stats, temp, arr[j - gaps[gap]]) == -1) { // == -1 normal // 1 reverse order
                arr[j] = move(stats, arr[j - gaps[gap]]);
                j -= gaps[gap];
            }
            arr[j] = move(stats, temp);
        }
    }
    return;
}
