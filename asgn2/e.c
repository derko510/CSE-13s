#include "mathlib.h"

#include <stdio.h>

// static global var for e_terms
static int terms = 1; // 1 to account for the 0!

// approximate the value of e using the Taylor series presented in §3 and track the number of computed terms by means of a static variable local to the file
double e(void) {
    terms = 1; //terms to 1 to reset after each call
    double previous = 1.0;
    double new = 0.0;
    double e = 1.0;
    for (int k = 1; (1 / new) > EPSILON; k++) { // can't start at one since divide by 0 error
        new = previous *k; //calculate the current term in the series
        previous = new; // update the previous. this basically calculates factorial
        e += 1 / new; // add the current term to the approximation of e
        terms += 1;
    }
    return e;
}

// return the number of computed terms.
int e_terms(void) {
    return terms;
}
