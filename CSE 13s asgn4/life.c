#include "universe.h"

#include <ctype.h>
#include <inttypes.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DELAY   50000
#define OPTIONS "tsn:i:o:h"

// helper functions
void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "    Conway's Game of Life\n\n"
        "USAGE\n"
        "    %s [tsn] [-n number] [-i file] [-o file]\n\n"
        "OPTIONS\n"
        "    -t             Create your universe as a toroidal\n"
        "    -s             Silent - do not use animate the evolution\n"
        "    -n {number}    Number of generations [default: 100]\n"
        "    -i {file}      Input file [default: stdin]\n"
        "    -o {file}      Output file [default: stdout]\n"
        "    -h             Display help\n",
        exec);
}

void swap_universes(Universe **universeA, Universe **universeB) {
    Universe *temp = *universeA;
    *universeA = *universeB;
    *universeB = temp;
}

int count_elements(FILE *infile) {
    int count = 0; // Initialize the count to 0
    int element;
    // Loop until there are no more elements to read from the file
    while (fscanf(infile, "%d", &element) == 1) {
        count++; // Increase the count for each element read
        if (getc(infile) == '\n') { // Check if the next character is a newline
            break; // If so, break out of the loop
        }
    }
    return count; // Return the final count
}

int main(int argc, char *argv[]) {
    // variables for the options and arguments
    int opt = 0;

    // defaults
    int generations = 100;
    bool toroidal = FALSE;
    bool silent = FALSE;

    // files
    FILE *infile = stdin; //stdin default
    FILE *outfile = stdout; //stdout deafult
    char *intarg = NULL;
    char *outtarg = NULL;

    // 1 parse the options using getopt
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't': toroidal = TRUE; break;
        case 's': silent = TRUE; break;
        case 'n': generations = atoi(optarg); break;
        case 'i': intarg = optarg; break;
        case 'o': outtarg = optarg; break;
        case 'h': usage(argv[0]); return 0;
        default: usage(argv[0]); return -1;
        }
    }

    // open outfile for usage
    outfile = fopen(outtarg, "w");

    // generations error handling
    if (generations < 0) {
        printf("Generations Invalid.\n");
        return -1;
    }

    // 2 open and reading with file
    if (intarg != NULL) {
        infile = fopen(intarg, "r");
        if (infile == NULL) { // check if file is valid
            printf("Error opening %s.\n", intarg);
            return -1;
        }
    }

    // 2 read the grid size
    uint32_t row, col;
    if (infile != stdin) { // when the infile is not stdin
        if (count_elements(infile) != 2) {
            printf("Malformed input.\n");
            return -1;
        }
    }
    if (infile != stdin) {
        fseek(infile, 0, SEEK_SET); // go back to line 1
    }
    fscanf(infile, "%u %u", &row, &col);

    // 3 two universes
    Universe *universeA = uv_create(row, col, toroidal);
    Universe *universeB = uv_create(row, col, toroidal);

    // 4 populate
    if (uv_populate(universeA, infile)
        == FALSE) { // print Malformed input when uv populate is false
        printf("Malformed input.\n"); // idea was from Ben Grant in discord (190n#1979)
        return -1;
    }
    uv_populate(universeA, infile);

    // 5 setup the ncurses screen
    if (silent == FALSE) {
        initscr();
        curs_set(FALSE);
    }
    for (int iters = 0; iters < generations; iters++) {
        clear();
        for (uint32_t row = 0; row < uv_rows(universeA); row++) {
            for (uint32_t col = 0; col < uv_cols(universeA); col++) {
                if (uv_get_cell(universeA, row, col)) { // check if theres a cell there
                    mvprintw(row, col, "o"); // prints the cell
                }
                // rule 1
                int neighbors = uv_census(universeA, row, col);
                if (uv_get_cell(universeA, row, col)) { // if alive
                    if (neighbors == 2 || neighbors == 3) { // if 2 or 3 neighbors
                        uv_live_cell(universeB, row, col); // set alive
                    } else {
                        uv_dead_cell(universeB, row, col); // rule 3 else everyone else dies
                    }
                } else {
                    if (neighbors == 3) { // rule 2
                        uv_live_cell(universeB, row, col); // set alive
                    } else {
                        uv_dead_cell(universeB, row, col); // rule 3 else everyone else dies
                    }
                }
            }
        }
        swap_universes(&universeA, &universeB);
        if (silent == FALSE) {
            refresh();
            usleep(DELAY);
        }
    }
    endwin(); // 7 close the screen

    // print the end image if no outfile
    if (outtarg == NULL) {
        for (uint32_t row = 0; row < uv_rows(universeA); row++) {
            for (uint32_t col = 0; col < uv_cols(universeA); col++) {
                if (uv_get_cell(universeA, row, col)) {
                    printf("o");
                } else {
                    printf(".");
                }
            }
            printf("\n");
        }
    }

    if (outtarg != NULL) {
        uv_print(universeA, outfile); // 8 print universe to outfile
    }

    fclose(infile); // close the files

    //cleaning
    uv_delete(universeA);
    uv_delete(universeB);
    return 0;
}
