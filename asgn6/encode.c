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

#define OPTIONS "hvo:i:"

void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Compresses files using the LZ78 compression algorithm.\n"
        "   Compressed files are decompressed with the corresponding decoder.\n\n"
        "USAGE\n"
        "   %s [-vh] [-i input] [-o output]\n\n"
        "OPTIONS\n"
        "   -v          Display compression statistics\n"
        "   -i input    Specify input to compress (stdin by default)\n"
        "   -o output   Specify output of compressed input (stdout by default)\n"
        "   -h          Display program help and usage\n",
        exec);
}

int bit_length(uint16_t code) {
    uint16_t count = 0;
    while (code) {
        count++;
        code >>= 1;
    }
    return count;
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

    struct stat stats; // declare a struct stat to hold information about the file
    fstat(infile, &stats); // obtain information about the input file and store it in stats
    fchmod(outfile,
        stats
            .st_mode); // use the file mode information from stats to set the permission mode for the output file

    FileHeader header = { 0 }; // declare a FileHeader struct and initialize all members to zero
    header.magic = MAGIC;
    header.protection = stats.st_mode;

    write_header(outfile, &header);

    // encoding
    TrieNode *root = trie_create();
    TrieNode *curr_node = root;
    TrieNode *prev_node = NULL;
    uint16_t next_code = START_CODE;
    uint8_t curr_sym = 0;
    uint8_t prev_sym = 0;
    while (read_sym(infile, &curr_sym)) {
        TrieNode *next_node = trie_step(curr_node, curr_sym);
        if (next_node) {
            prev_node = curr_node;
            curr_node = next_node;
        } else {
            write_pair(outfile, curr_node->code, curr_sym, bit_length(next_code));
            curr_node->children[curr_sym] = trie_node_create(next_code);
            curr_node = root;
            next_code = next_code + 1;
        }

        if (next_code == MAX_CODE) {
            trie_reset(root);
            curr_node = root;
            next_code = START_CODE;
        }
        prev_sym = curr_sym;
    }
    if (curr_node != root) {
        write_pair(outfile, prev_node->code, prev_sym, bit_length(next_code));
        next_code = (next_code + 1) % MAX_CODE;
    }
    write_pair(outfile, STOP_CODE, 0, bit_length(next_code));
    flush_pairs(outfile);

    // psuedocode from pdf

    if (verbose == true) {
        total_bits = (total_bits + 7) / 8;
        double ratio = 1.0 - (double) total_bits / (double) total_syms;
        printf("Compressed file size: %lu bytes\n", total_bits);
        printf("Uncompressed file size: %lu bytes\n", total_syms);
        printf("Compression ratio: %.2f%%\n", 100 * ratio);
    }

    // clean and closing
    trie_delete(root);
    close(infile);
    close(outfile);
    return 0;
}
