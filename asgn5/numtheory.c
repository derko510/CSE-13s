#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "numtheory.h"
#include "randstate.h"

// code reused from my FALL 2022 quarter
// reworked since some code wasn't functional
// mod_inverse and is_prime changed

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    mpz_t t, tmpb, tmpa;
    mpz_inits(t, tmpb, tmpa, NULL);

    mpz_set(tmpb, b);
    mpz_set(tmpa, a);
    while (mpz_cmp_ui(tmpb, 0) != 0) {
        mpz_set(t, tmpb);
        mpz_mod(tmpb, tmpa, tmpb);
        mpz_set(tmpa, t);
    }
    mpz_set(g, tmpa);
    mpz_clears(t, tmpb, tmpa, NULL);
}

void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    mpz_t r, r2, t, t2, q, tmp1, tmp2;
    mpz_inits(r, r2, t, t2, q, tmp1, tmp2, NULL);
    mpz_set(r, n); // r = n
    mpz_set(r2, a); // r' = a
    mpz_set_ui(t, 0); // t = 0
    mpz_set_ui(t2, 1); // t' = 1
    while (mpz_cmp_ui(r2, 0) != 0) { // while r' != 0
        mpz_fdiv_q(q, r, r2); // q = r / r'

        mpz_set(tmp1, r2); // setting tmp for r'
        mpz_mod(r2, r, r2); // r' = r mod r'
        mpz_set(r, tmp1); // r = r'

        mpz_set(tmp2, t2); // setting tmp for t'
        mpz_mul(tmp1, q, t2); // tmp1 = q * t'
        mpz_sub(t2, t, tmp1); // t' = t - q * t'
        mpz_set(t, tmp2); // t = t'
    }
    if (mpz_cmp_ui(r, 1) > 0) { // if r > 1
        mpz_set_ui(o, 0);
    } else {
        if (mpz_cmp_ui(t, 0) < 0) { // if t < 0
            mpz_add(t, t, n);
        }
        mpz_set(o, t);
    }
    mpz_clears(r, r2, t, t2, q, tmp1, tmp2, NULL);
}

// given psuedocode
void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    mpz_t p, exp, mod, v;
    mpz_inits(p, exp, mod, v, NULL);
    mpz_set_ui(v, 1);
    mpz_set(p, a);
    mpz_set(exp, d);
    mpz_set(mod, n);
    while (mpz_cmp_ui(exp, 0) > 0) {
        if (mpz_odd_p(exp) != 0) {
            mpz_mul(v, v, p);
            mpz_mod(v, v, n); // v = (v*p) % n;
        }
        mpz_mul(p, p, p);
        mpz_mod(p, p, n); // p = (p*p) % n;
        mpz_fdiv_q_ui(exp, exp, 2); // d = d / 2;
    }
    mpz_set(o, v);
    mpz_clears(p, exp, mod, v, NULL);
}

bool is_prime(const mpz_t n, uint64_t iters) {
    // initializing vars
    mpz_t a, y, r, j, s, tmpn1, tmps, pos, n1, n3, div, tmpn;
    mpz_inits(a, y, r, j, s, tmpn1, tmps, pos, n1, n3, div, tmpn, NULL);
    mpz_mod_ui(pos, n, 2);
    mpz_sub_ui(n1, n, 1);
    mpz_sub_ui(n3, n, 2);
    mpz_set(tmpn, n);
    mpz_set_ui(div, 1);
    if (mpz_cmp_ui(tmpn, 3) == 0 || mpz_cmp_ui(tmpn, 2) == 0) { // special case
        mpz_clears(a, y, r, j, s, pos, n1, n3, div, tmpn, NULL);
        return true;
    } else if (mpz_cmp_ui(tmpn, 1) == 0 || mpz_cmp_ui(tmpn, 4) == 0
               || mpz_cmp_ui(tmpn, 0) == 0) { // special case
        mpz_clears(a, y, r, j, s, pos, n1, n3, div, tmpn, NULL);
        return false;
    } else if (mpz_even_p(pos)) { // if even case
        mpz_clears(a, y, r, j, s, pos, n1, n3, div, tmpn, NULL);
        return false;
    } else if (mpz_divisible_ui_p(n, 5)) {
        mpz_clears(a, y, r, j, s, pos, n1, n3, div, tmpn, NULL);
        return false;
    }
    mpz_sub_ui(tmpn, n, 1);
    while (mpz_odd_p(r) == 0) { // dividing by 2 until r is odd
        mpz_div(r, tmpn, div);
        mpz_mul_ui(div, div, 2);
        mpz_add_ui(s, s, 1); // adding one for each iters
    }
    mpz_sub_ui(s, s, 1);
    mpz_sub_ui(tmps, s, 1);
    mpz_sub_ui(tmpn1, n, 1);
    for (uint64_t k = 1; k < iters; k++) {
        mpz_urandomm(a, state, n3); // n-2 which is n-3
        mpz_add_ui(a, a, 2); // creates [2,n-2]
        pow_mod(y, a, r, n);
        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, tmpn1) != 0) {
            mpz_set_ui(j, 1);
            while (mpz_cmp(j, tmps) <= 0 && mpz_cmp(y, tmpn1) != 0) {
                mpz_mul(y, y, y);
                mpz_mod(y, y, n); // pow mod
                if (mpz_cmp_ui(y, 1) == 0) { // y==1
                    mpz_clears(a, y, r, j, s, tmpn1, tmps, pos, n1, n3, div, tmpn, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, tmpn1) != 0) { // y != n-1
                mpz_clears(a, y, r, j, s, tmpn1, tmps, pos, n1, n3, div, tmpn, NULL);
                return false;
            }
        }
    }
    mpz_clears(a, y, r, j, s, tmpn1, tmps, pos, n1, n3, div, tmpn, NULL);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    int prime_found = 0;
    while (!prime_found) {
        // generate a random number of the desired bit size
        mpz_urandomb(p, state, bits);
        // set the high bit to ensure the number has the desired bit size
        mpz_setbit(p, bits - 1);
        // test the primality of the number using is_prime
        if (is_prime(p, iters)) {
            prime_found = 1;
        }
    }
}
