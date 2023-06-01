#include "mathlib.h"

#include <stdio.h>

// intitizle global var for terms
static int terms = 0;

double pi_madhava(void) {
    terms = 0; //terms to 0 to reset after each call
    double pi = 0;
    double denominator = 1;
    for (int k = 0; absolute(1 / denominator) > EPSILON; k++) { // stops loop when
        double sign = (k % 2 == 0)
                          ? 1
                          : -1; // calculates -1 or 1 when k is negative or postive respectively
        pi += sign / (2 * k + 1) / denominator;
        denominator *= 3; // each time it loops it will x3 which is 3^k
        terms++;
    }
    return pi * sqrt_newton(12); // times sqrt to cancel out to only pi
}

int pi_madhava_terms(void) {
    return terms;
}
