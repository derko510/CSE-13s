#include "universe.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    // allocate memory for the Universe struct
    Universe *uv = (Universe *) calloc(1, sizeof(Universe));
    // check if allocation was successful
    if (!uv) {
        return NULL;
    }
    // set the number of rows and columns for the grid
    uv->rows = rows;
    uv->cols = cols;
    // set whether the grid is toroidal or not
    uv->toroidal = toroidal;
    // allocate memory for the 2D grid of booleans
    uv->grid = (bool **) calloc(rows, sizeof(bool *));
    // check if allocation was successful
    if (!uv->grid) {
        free(uv);
        return NULL;
    }
    // allocate memory for each row of the grid
    for (uint32_t r = 0; r < rows; r += 1) {
        uv->grid[r] = (bool *) calloc(cols, sizeof(bool));
        // check if allocation was successful
        if (!uv->grid[r]) {
            // free memory for previous rows if allocation failed
            for (uint32_t i = 0; i < r; i += 1) {
                free(uv->grid[i]);
            }
            // free memory for the grid and the Universe struct
            free(uv->grid);
            free(uv);
            return NULL;
        }
    }
    // return the newly created Universe
    return uv;
}

void uv_delete(Universe *u) {
    if (!u) {
        return;
    }
    for (uint32_t r = 0; r < u->rows; r += 1) {
        free(u->grid[r]);
    }
    free(u->grid);
    free(u);
}

uint32_t uv_rows(Universe *u) {
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r >= u->rows || c >= u->cols) {
        return;
    }
    u->grid[r][c] = true;
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r >= u->rows || c >= u->cols) {
        return;
    }
    u->grid[r][c] = false;
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r >= u->rows || c >= u->cols) {
        if (u->toroidal) {
            r = r % u->rows;
            c = c % u->cols;
        } else {
            return false;
        }
    }
    return u->grid[r][c];
}

bool uv_populate(Universe *u, FILE *infile) {
    uint32_t rows, cols;
    while (fscanf(infile, "%u %u", &rows, &cols) == 2) {
        if (rows > uv_rows(u) || cols > uv_cols(u) || rows < 0
            || cols < 0) { // if rows are greater than the grid
            return false;
        }
        uv_live_cell(u, rows, cols);
    }
    return true;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    int live_neighbors = 0;
    int row, col;
    // loop through all the neighboring cells
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) { // skip the cell itself
                continue;
            }
            if (u->toroidal) { // if the universe is toroidal
                // calculate the row and column indices by wrapping around the grid
                row = (r + i + u->rows) % u->rows;
                col = (c + j + u->cols) % u->cols;
            } else { // if the universe is not toroidal
                row = r + i;
                col = c + j;
                if (row < 0 || row >= (int) u->rows || col < 0 || col >= (int) u->cols) {
                    // skip cells that are outside the grid
                    continue;
                }
            }
            if (u->grid[row][col]) { // if the current cell is alive
                live_neighbors++;
            }
        }
    }
    return live_neighbors;
}

void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t row = 0; row < u->rows; row++) {
        for (uint32_t col = 0; col < u->cols; col++) {
            char cell = u->grid[row][col] ? 'o' : '.';
            fputc(cell, outfile);
        }
        fputc('\n', outfile);
    }
}
