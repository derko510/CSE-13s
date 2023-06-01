#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "endian.h"
#include "code.h"

// global vars from header
uint64_t total_syms = 0;
uint64_t total_bits = 0;

// 2 but for functions
static uint8_t buffer[BLOCK];
static uint8_t pair_buffer[BLOCK];
static int pair_index = 0;

// read
static int buffer_pos = 0;
static int buffer_size = 0;
static uint8_t bit_buffer = 0;
static int bit_pos = 0;

int read_bytes(int infile, uint8_t *buf, int to_read) {
    int total_read = 0;
    int bytes_read = 0;
    while (total_read < to_read
           && (bytes_read = read(infile, buf + total_read, to_read - total_read)) > 0) {
        total_read += bytes_read; // increment total bytes read
    }
    return total_read;
}

int write_bytes(int outfile, uint8_t *buf, int to_write) {
    int total_written = 0;
    int bytes_written = 0;
    while (total_written < to_write
           && (bytes_written = write(outfile, buf + total_written, to_write - total_written)) > 0) {
        total_written += bytes_written; // increment total bytes written
    }
    return total_written;
}

void read_header(int infile, FileHeader *header) {
    // read sizeof(FileHeader) bytes into the header
    read_bytes(infile, (uint8_t *) header, sizeof(FileHeader));
    // swap endianness
    if (header->magic != MAGIC) {
        header->magic = swap32(header->magic); // swap the endianness of the magic number
        header->protection
            = swap16(header->protection); // swap the endianness of the protection number
    }
    if (header->magic != MAGIC) {
        printf("Bad magic number.\n");
        exit(1);
    }
    total_bits += 8 * sizeof(FileHeader);
}

void write_header(int outfile, FileHeader *header) {
    if (header == NULL) { // check that header is not NULL
        return;
    }
    // swap endianness
    if (header->magic != MAGIC) {
        header->magic = swap32(header->magic); // swap the endianness of the magic number
        header->protection
            = swap16(header->protection); // swap the endianness of the protection number
    }
    // write sizeof(FileHeader) bytes from the header
    write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader));
    total_bits += 8 * sizeof(FileHeader);
}

bool read_sym(int infile, uint8_t *sym) {
    static int index = 0;
    static int end = 0;
    // if we've reached the end of the buffer, read another block
    if (index >= end) {
        end = read_bytes(infile, buffer, BLOCK); // read BLOCK bytes into buffer
        index = 0; // reset the index to the start of buffer
    }
    // if we've reached the end of the input file, return false
    if (end == 0) {
        return false;
    }
    // read the next symbol from the buffer
    *sym = buffer[index++];
    total_syms++;
    return true;
}

void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) {
    total_bits += bitlen + 8;
    // write the code to the buffer
    for (int i = 0; i < bitlen; i++) {
        int bit = (code >> i) & 1;
        pair_buffer[pair_index / 8] |= bit << (pair_index % 8);
        pair_index++;
        // flush the buffer if it is full
        if (pair_index >= BLOCK * 8) {
            flush_pairs(outfile);
        }
    }
    // write the symbol to the buffer
    for (int i = 0; i < 8; i++) {
        int bit = (sym >> i) & 1;
        pair_buffer[pair_index / 8] |= bit << (pair_index % 8);
        pair_index++;
        // flush the buffer if it is full
        if (pair_index >= BLOCK * 8) {
            flush_pairs(outfile);
        }
    }
}

void flush_pairs(int outfile) {
    // calculate the number of bytes to write
    int bytes_to_write = (pair_index + 7) / 8;
    // write the pairs to the output file
    if (bytes_to_write > 0) {
        if (write_bytes(outfile, pair_buffer, bytes_to_write) != bytes_to_write) {
            exit(1);
        }
    }
}

bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) {
    total_bits += bitlen + 8;
    while (true) {
        // check if the buffer is empty
        if (buffer_pos >= buffer_size) {
            // read a new block of pairs
            buffer_size = read_bytes(infile, buffer, BLOCK);
            buffer_pos = 0;
        }
        // check if there are no more pairs left to read
        if (buffer_pos >= buffer_size) {
            return false;
        }
        // read the code
        *code = 0;
        for (int i = 0; i < bitlen; i++) {
            // read a bit from the buffer
            if (bit_pos == 0) {
                bit_buffer = buffer[buffer_pos++];
            }
            int bit = (bit_buffer >> bit_pos) & 1;
            bit_pos = (bit_pos + 1) % 8;
            // add the bit to the code
            *code |= (bit << i);
        }
        // read the symbol
        *sym = 0;
        for (int i = 0; i < 8; i++) {
            // read a bit from the buffer
            if (bit_pos == 0) {
                bit_buffer = buffer[buffer_pos++];
            }
            int bit = (bit_buffer >> bit_pos) & 1;
            bit_pos = (bit_pos + 1) % 8;
            // add the bit to the symbol
            *sym |= (bit << i);
        }
        // check if the code is the stop code
        if (*code == STOP_CODE) {
            return false;
        } else {
            return true;
        }
    }
}

void write_word(int outfile, Word *w) {
    total_syms += w->len;
    static int buffer_pos = 0;
    for (uint32_t i = 0; i < w->len; i++) {
        buffer[buffer_pos++] = w->syms[i];
        // flush buffer if it's full
        if (buffer_pos >= BLOCK) {
            int bytes_written = write_bytes(outfile, buffer, BLOCK);
            if (bytes_written < 0) {
                exit(1);
            }
            buffer_pos = 0;
        }
    }
    // flush any remaining symbols in the buffer
    if (buffer_pos > 0) {
        int bytes_written = write_bytes(outfile, buffer, buffer_pos);
        if (bytes_written < 0) {
            exit(1);
        }
        buffer_pos = 0;
    }
}

void flush_words(int outfile) {
    static int buf_index = 0; // current position in the buffer
    // check if there are any words in the buffer to write
    if (buf_index > 0) {
        // write the words to the output file
        int bytes_written = write_bytes(outfile, buffer, buf_index);
        if (bytes_written < 0) { // check for errors during writing
            exit(1);
        }
        // reset the buffer
        buf_index = 0;
    }
}
