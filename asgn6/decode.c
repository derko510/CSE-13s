#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include "io.h"
#include "trie.h"
#include "word.h"
#include "code.h"

#define OPTIONS "vi:o:h"

int bit_length(uint16_t code) {
    uint16_t count = 0;
    while (code) {
        count++;
        code >>= 1;
    }
    return count;
}

void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Decompresses files with the LZ78 decompression algorithm.\n"
        "   Used with files compressed with the corresponding encoder.\n\n"
        "USAGE\n"
        "   %s [-vh] [-i input] [-o output]\n\n"
        "OPTIONS\n"
        "   -v          Display decompression statistics\n"
        "   -i input    Specify input to decompress (stdin by default)\n"
        "   -o output   Specify output of decompressed input (stdout by default)\n"
        "   -h          Display program usage\n",
        exec);
}

int main(int argc, char **argv) {
    int opt = 0;

    // defaults
    int infile = STDIN_FILENO;
    int outfile = STDOUT_FILENO;
    bool verbose = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': usage(argv[0]); return 0;
        case 'v': verbose = true; break;
        case 'i':
            infile = open(optarg, O_RDONLY); // in the test file
            if (infile == -1) {
                printf("Failed to open %s.\n", optarg);
                return -1;
            }
            break;
        case 'o':
            outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0644); // in the test file
            if (outfile == -1) {
                printf("Failed to open %s.\n", optarg);
                return -1;
            }
            break;
        default: usage(argv[0]); return -1;
        }
    }

    // declare a FileHeader struct
    FileHeader header;
    read_header(infile, &header);
    fchmod(outfile, header.protection);

    // decoding
    // create word table and initialize
    WordTable *table = wt_create();
    uint8_t curr_sym = 0;
    uint16_t curr_code = 0;
    uint16_t next_code = START_CODE;
    // decompress file
    while (read_pair(infile, &curr_code, &curr_sym, bit_length(next_code))) {
        // construct new word and add to table
        table[next_code] = word_append_sym(table[curr_code], curr_sym);

        // write new word to output file
        write_word(outfile, table[next_code]);
        // increment next code and check for table reset
        next_code++;
        if (next_code == MAX_CODE) {
            wt_reset(table);
            next_code = START_CODE;
        }
    }
    // flush any remaining words to output file
    flush_words(outfile);
    wt_delete(table);

    // psuedocode from pdf

    if (verbose == true) {
        total_bits = (total_bits + 7) / 8;
        double ratio = 1.0 - (double) total_bits / (double) total_syms;
        printf("Compressed file size: %lu bytes\n", total_bits);
        printf("Uncompressed file size: %lu bytes\n", total_syms);
        printf("Compression ratio: %.2f%%\n", 100 * ratio);
    }

    // free memory used by word table
    close(infile);
    close(outfile);
    return 0;
}
