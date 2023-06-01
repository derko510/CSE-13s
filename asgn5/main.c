#include "numtheory.h"
#include "randstate.h"

#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    // declare our numbers
    mpz_t a, b, d, op1, op2, result, result2;
    // gmp_randstate_t state;
    //  initialize them
    mpz_inits(a, b, d, op1, op2, result, result2, NULL);
    // set the 2 numbers we compute GCD of
    mpz_set_ui(a, 3);
    mpz_set_ui(b, 6);
    mpz_set_ui(d, 125215);
    mpz_set_ui(op1, 125123);
    mpz_set_ui(op2, 97);

    // gmp_printf("%s\n", is_prime(a, 5) ? "true prime" : "false composite");
    // //   gmp_printf("%s\n", is_prime(b, 5) ? "true prime" : "false composite");
    // //   gmp_printf("%s\n", is_prime(d, 5) ? "true prime" : "false composite");
    // //   gmp_printf("%s\n", is_prime(op1, 5) ? "true prime" : "false composite");
    // //   gmp_printf("%s\n", is_prime(op2, 5) ? "true prime" : "false composite");

    mpz_invert(result, a, b);
    mod_inverse(result2, a, b);
    gmp_printf("%Zd\n", result);
    gmp_printf("%Zd\n", result2);
    //   uint64_t seed = 1;
    //   randstate_init(seed);
    //   make_prime(p, bits, iters);
    //   gmp_printf("%Zd\n", p);
    // free all our memory
    mpz_clears(a, b, d, op1, op2, NULL);
    return 0;
}
