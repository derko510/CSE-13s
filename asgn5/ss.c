#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gmp.h>

#include "ss.h"
#include "numtheory.h"
#include "randstate.h"

void lcm(mpz_t o, const mpz_t a, const mpz_t b) { //abs(a*b)/gcd(a, b)
    mpz_t val, x, tmpa, tmpb; //initilzing vars
    mpz_inits(x, val, tmpa, tmpb, NULL);
    mpz_set(tmpa, a);
    mpz_set(tmpb, b);
    mpz_mul(val, tmpa, tmpb);
    mpz_abs(val, val); //abs (a*b)
    gcd(x, a, b); //gcd(a,b)
    mpz_fdiv_q(o, val, x);
    mpz_clears(x, val, tmpa, tmpb, NULL);
}

void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {
    // calculate the minimum number of bits for p
    uint64_t pbits = (nbits / 5) + (uint64_t) (random() % (nbits / 5));
    // calculate the number of bits for q
    uint64_t qbits = nbits - 2 * pbits;

    // generate primes p and q using make_prime
    make_prime(p, pbits, iters);
    make_prime(q, qbits, iters);

    // check that p does not divide q-1 and q does not divide p-1
    mpz_t p_minus_one, q_minus_one;
    mpz_inits(p_minus_one, q_minus_one, NULL);
    mpz_sub_ui(p_minus_one, p, 1);
    mpz_sub_ui(q_minus_one, q, 1);
    if (mpz_divisible_p(p_minus_one, q) && mpz_divisible_p(q_minus_one, p)) {
        // if both conditions is true, regenerate primes p and q until they satisfy the conditions
        ss_make_pub(p, q, n, nbits, iters);
        return;
    }

    // n = p^2 * q
    mpz_mul(n, p, p);
    mpz_mul(n, n, q);

    // clears
    mpz_clears(p_minus_one, q_minus_one, NULL);
}

void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%s\n", username);
}

void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%s\n", username);
}

void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {
    // lambda(pq)
    mpz_t lambda, tmpp, tmpq, n;
    mpz_inits(lambda, tmpp, tmpq, n, NULL);

    // pq value
    mpz_mul(pq, p, q);

    mpz_sub_ui(tmpp, p, 1);
    mpz_sub_ui(tmpq, q, 1);
    lcm(lambda, tmpp, tmpq);

    // n = p^2 * q
    mpz_mul(n, p, p);
    mpz_mul(n, n, q);

    // d = n^-1 mod lambda(pq)
    mod_inverse(d, n, lambda);

    // clear memory
    mpz_clears(lambda, tmpp, tmpq, n, NULL);
}

void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", pq);
    gmp_fprintf(pvfile, "%Zx\n", d);
}

void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", pq);
    gmp_fscanf(pvfile, "%Zx\n", d);
}

void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    pow_mod(c, m, n, n);
}

void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    pow_mod(m, c, d, pq);
}

void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    uint64_t k;
    uint64_t j;
    mpz_t m, c, sqn;
    mpz_inits(m, c, sqn, NULL);
    mpz_sqrt(sqn, n);
    k = (mpz_sizeinbase(sqn, 2) - 1) / 8; // size of block
    uint8_t *arr
        = (uint8_t *) calloc(k, sizeof(uint8_t)); // allocate memory for array to hold block
    // set the first byte of the block to 0xFF
    arr[0] = 0xFF;
    // read the input file in blocks of size k-1
    for (;;) {
        j = fread(&arr[1], 1, k - 1, infile);
        // if end of file is reached, break
        if (j == 0) {
            break;
        }
        // import block into m
        mpz_import(m, j + 1, 1, sizeof(uint8_t), 1, 0, arr);
        // encrypt block using ss_encrypt function
        ss_encrypt(c, m, n);
        // write the encrypted number to outfile as a hexstring followed by a newline
        gmp_fprintf(outfile, "%Zx\n", c);
    }
    // free memory
    free(arr);
    // clear variables
    mpz_clears(m, c, sqn, NULL);
}

void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t pq, const mpz_t d) {
    uint64_t k = (mpz_sizeinbase(pq, 2) - 1) / 8; // set size of block
    uint8_t *block = (uint8_t *) calloc(k, sizeof(uint8_t)); // allocate memory for block
    // initialize variables for decryption
    mpz_t c, m;
    size_t j;
    mpz_inits(c, m, NULL);
    // read until end of input file
    while (!feof(infile)) {
        // read ciphertext from input file
        gmp_fscanf(infile, "%Zx\n", c);
        // decrypt ciphertext
        ss_decrypt(m, c, pq, d);
        // convert m to bytes
        mpz_export(&block[0], &j, 1, sizeof(uint8_t), 1, 0, m);
        // write decrypted bytes to output file
        fwrite(&block[1], sizeof(uint8_t), j - 1, outfile);
    }
    // free memory
    free(block);
    // clear variables
    mpz_clears(c, m, NULL);
}
