#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>

#include "randstate.h"
#include "ss.h"

#define OPTIONS "i:o:n:vh"

void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Encrypts data using SS encryption.\n"
        "   Encrypted data is decrypted by the decrypt program.\n\n"
        "USAGE\n"
        "  %s [OPTIONS]\n\n"
        "OPTIONS\n"
        "  -h            Display program help and usage.\n"
        "  -v            Display verbose program output.\n"
        "  -i infile     Input file of data to encrypt (default: stdin).\n"
        "  -o outfile    Output file for encrypted data (default: stdout).\n"
        "  -n pbfile     Public key file (default: ss.pub).\n",
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
    FILE *pbfile = NULL;
    char *keyfile = "ss.pub";
    char username[256] = { 0 };

    // initializing variables for GMP types
    mpz_t user, e, n, s, m;
    mpz_inits(user, e, n, s, m, NULL);

    // get options from command line
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': input = optarg; break; // set input file
        case 'o': output = optarg; break; // set output file
        case 'n': keyfile = optarg; break; // set public key file
        case 'v': verbose = 1; break; // turn on verbose output
        case 'h': usage(argv[0]); return 0; // print out help and exit
        default: usage(argv[0]); return -1;
        }
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

    // check if a public key file exists
    FILE *pubkey = (fopen(keyfile, "r"));
    if (pubkey == NULL) {
        printf("encrypt: couldn't open %s to read public key.\n", keyfile);
        return -1;
    } else if (pubkey != NULL) { // check if a public key file was specified and it exists
        pbfile = fopen(keyfile, "r");
        ss_read_pub(n, username, pbfile); // read public key
    }

    if (verbose == 1) { // checks if verbose was turned on
        gmp_printf("user = %s\n", username);
        gmp_printf("n (%d bits): %Zd\n", mpz_sizeinbase(n, 2), n);
    }

    // encrypts the contents of the input file
    ss_encrypt_file(infile, outfile, n);

    // closing all files
    fclose(outfile);
    fclose(infile);
    fclose(pbfile);
    mpz_clears(user, e, n, s, m, NULL);
    return 0;
}
