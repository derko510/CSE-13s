#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>

#include "randstate.h"
#include "ss.h"

#define OPTIONS "i:o:n:vh"

void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Decrypts data using SS decryption.\n"
        "   Encrypted data is encrypted by the encrypt program.\n\n"
        "USAGE\n"
        "  %s [OPTIONS]\n\n"
        "OPTIONS\n"
        "  -h            Display program help and usage.\n"
        "  -v            Display verbose program output.\n"
        "  -i infile     Input file of data to decrypt (default: stdin).\n"
        "  -o outfile    Output file for decrypted data (default: stdout).\n"
        "  -n pvfile     Private key file (default: ss.priv).\n",
        exec);
}

int main(int argc, char **argv) {
    int opt = 0;
    int verbose = 0;

    // defaults
    char *input = NULL;
    char *output = NULL;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *privfile = NULL;
    char *keyfile = "ss.priv";

    // mpz inits
    mpz_t pq, d;
    mpz_inits(pq, d, NULL);

    // parse command line options
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': input = optarg; break; // set input file name
        case 'o': output = optarg; break; // set output file name
        case 'n': keyfile = optarg; break; // set private key file name
        case 'v': verbose = 1; break; // enable verbose output
        case 'h': usage(argv[0]); return 0; // print usage and exit
        default: usage(argv[0]); return -1; // print usage and exit with error
        }
    }

    // read private key from file
    FILE *pubkey = (fopen(keyfile, "r"));
    if (pubkey == NULL) { // checks if keyfile exists
        printf("decrypt: couldn't open %s to read private key.\n", keyfile);
        return -1;
    } else {
        privfile = fopen(keyfile, "r"); // opens the priv key file
        ss_read_priv(pq, d, privfile); // read the private key from the file
    }

    // check if an input file was specified and if it exists
    if (input != NULL) {
        FILE *inputfile = fopen(input, "r");
        if (inputfile != NULL) {
            infile = inputfile;
        } else {
            fprintf(stderr, "Error: could not open input file %s\n", input);
            return -1;
        }
    }

    // check if an output file was specified and if it exists
    if (output != NULL) {
        FILE *outputfile = fopen(output, "w");
        if (outputfile != NULL) {
            outfile = outputfile;
        } else {
            fprintf(stderr, "Error: could not open output file %s\n", output);
            return -1;
        }
    }

    if (verbose == 1) { // prints verbose if on
        gmp_printf("pq (%d bits): %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d (%d bits): %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    // decrypt
    ss_decrypt_file(infile, outfile, d, pq);

    // closing and clears
    fclose(outfile);
    fclose(infile);
    fclose(privfile);
    mpz_clears(pq, d, NULL);
    return 0;
}
