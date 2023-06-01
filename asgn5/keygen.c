#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <gmp.h>

#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

#define OPTIONS "s:b:i:n:d:vh"

// code

void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "    Generates an SS public/private key pair.\n\n"
        "USAGE\n"
        "    %s [OPTIONS]\n\n"
        "OPTIONS\n"
        "    -h              Display program help and usage.\n"
        "    -v              Display verbose program output.\n"
        "    -b bits         Minimum bits needed for public key n (default: 256).\n"
        "    -i iterations   Miller-Rabin iterations for testing primes (default: 50).\n"
        "    -n pbfile       Public key file (default: ss.pub).\n"
        "    -d pvfile       Private key file (default: ss.priv).\n"
        "    -s seed         Random seed for testing.\n",
        exec);
}

int main(int argc, char **argv) {
    // defaults
    uint64_t bits = 256;
    uint64_t iters = 50;
    int seed;

    int seed_flag = 0;
    int verbose = 0;
    FILE *pvfile = NULL;
    FILE *pbfile = NULL;
    char *pub = "ss.pub";
    char *priv = "ss.priv";
    // initializating vars
    mpz_t p, q, n, e, d, pq, user;
    mpz_inits(p, q, n, e, d, pq, user, NULL);
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 's':
            seed = atoi(optarg);
            randstate_init(seed);
            seed_flag = 1;
            break;
        case 'b': bits = atoi(optarg); break;
        case 'i': iters = atoi(optarg); break;
        case 'n': pub = optarg; break;
        case 'd': priv = optarg; break;
        case 'v': verbose = 1; break;
        case 'h': usage(argv[0]); return 0;
        }
    }
    // opens the private and public key files for writing
    pbfile = fopen(pub, "w");
    pvfile = fopen(priv, "w");
    // gives permission to the user and not anyone else
    uint64_t pvnum = fileno(pvfile);
    fchmod(pvnum, S_IRUSR | S_IWUSR);

    // checking if there was -s input
    if (seed_flag == 0) {
        randstate_init(time(NULL)); // set to time since EPOCH when -s not active
    }

    // making
    ss_make_pub(p, q, n, bits, iters);
    ss_make_priv(d, pq, p, q);

    // store username as string
    char *username = getenv("USER"); // strdup Prof Long (Athanasius Kircher#1515)
    // writing
    ss_write_pub(n, username, pbfile);
    ss_write_priv(pq, d, pvfile);

    // prints the variables and their bit sizes if verbose is enabled
    if (verbose == 1) {
        gmp_printf("user = %s\n", username);
        gmp_printf("p  (%d bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q  (%d bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n  (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("d  (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
        gmp_printf("pq (%d bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
    }

    // close files
    fclose(pvfile);
    fclose(pbfile);

    // clears
    randstate_clear();
    mpz_clears(p, q, n, e, d, pq, user, NULL);
}
