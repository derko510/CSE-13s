#include "batcher.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "Hasbhqn:p:r:"

void random_arr_gen(uint32_t *arr, int seed, uint32_t size) {
    srandom(seed); //sets the seed of gen
    for (uint32_t i = 0; i < size; i++) {
        uint64_t num = random() & (1 << 30) - 1; // bit masks to 30 bits
        arr[i] = num;
    }
    return;
}

// reversed
// void reverse_arr(uint32_t *arr, uint32_t size) {
//     for (uint32_t i = 0; i < size / 2; i++) {
//         uint32_t temp = arr[i];
//         arr[i] = arr[size - i - 1];
//         arr[size - i - 1] = temp;
//     }
//     return;
// }

void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   A collection of comparison-based sorting algorithms.\n\n"
        "USAGE\n"
        "   %s [-Hasbhqn:p:r:] [-n size] [-p elements] [-r seed]\n\n"
        "OPTIONS\n"
        "   -H              Display program help and usage.\n"
        "   -a              Enable all sorts.\n"
        "   -s              Enable Shell Sort.\n"
        "   -b              Enable Batcher Sort.\n"
        "   -h              Enable Heap Sort.\n"
        "   -q              Enable Quick Sort.\n"
        "   -n size         Specify number of array elements (default: 100).\n"
        "   -p elements     Specify number of elements to print (default: 100).\n"
        "   -r seed         Specify random seed (default: 13371453).\n",
        exec);
}

int main(int argc, char **argv) {
    Stats stats;
    int opt = 0;
    //default values
    int seed = 13371453;
    uint32_t size = 100;
    uint32_t elements = 100;

    //setting sort values
    enum sorts { SHELL, BATCHER, HEAP, QUICK, END }; // From Long in discord
    char *sort_names[] = { "Shell Sort", "Batcher Sort", "Heap Sort", "Quick Sort" };

    Set set = set_empty();

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'H': usage(argv[0]); return 0;
        case 'a':
            set = set_insert(set, SHELL);
            set = set_insert(set, BATCHER);
            set = set_insert(set, HEAP);
            set = set_insert(set, QUICK);
            break;
        case 's': set = set_insert(set, SHELL); break;
        case 'b': set = set_insert(set, BATCHER); break;
        case 'h': set = set_insert(set, HEAP); break;
        case 'q': set = set_insert(set, QUICK); break;
        case 'n': size = strtoul(optarg, NULL, 10); break; // sets array size
        case 'p':
            elements = strtoul(optarg, NULL, 10);
            break; //Sets the number of elements to print
        case 'r': seed = strtoul(optarg, NULL, 10); break; // sets the seed #
        }
    }

    // checks to not print too many elements
    if (elements > size) {
        elements = size;
    }

    // if no sorts are entered prompt user to enter at least one sort
    if (set == 0) {
        printf("Select at least one sort to perform.\n");
        usage(argv[0]);
        return -1;
    }

    //initialize array
    uint32_t *random_arr;
    random_arr = malloc(size * sizeof(uint32_t)); // 1 create

    void (*sorts_ptr[4])(Stats * stats, uint32_t * arr,
        uint32_t size) // pointer to functions to know which one to call in for loop
        = { shell_sort, batcher_sort, heap_sort, quick_sort };

    for (int i = 0; i < END; i++) { //look through the enum sorts
        if (set_member(set, i)) {
            random_arr_gen(random_arr, seed, size); // 2 use
            // reverse_arr(random_arr, size); // reverse the array from random_arr_gen()
            sorts_ptr[i](&stats, random_arr, size); // ptr
            printf("%s, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", sort_names[i],
                size, stats.moves, stats.compares);
            for (uint32_t i = 0; i < elements; i++) {
                if (i % 5 == 0 && i != 0) { // creates columns of 5
                    printf("\n");
                }
                printf("%13" PRIu32, random_arr[i]);
                if (i == elements - 1) {
                    printf("\n");
                }
            }
        }
    }
    free(random_arr); // 3 destroy
    return 0;
}
