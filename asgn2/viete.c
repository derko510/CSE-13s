#include "mathlib.h"

#include <stdio.h>

// intitizle global var for terms
static int terms = 0;

double pi_viete(void) {
    terms = 0; //terms to 0 to reset after each call
    double result = 1;
    double an = sqrt_newton(2);
    double final = sqrt_newton(2);
    double twodiv = 2;
    for (int i = 1; absolute(2 - an) >= EPSILON;
         i++) { // looping until the iteration changes by smaller than EPSILON
        an = sqrt_newton(an + 2); // calculate next square root using Newton's method
        final *= an; // update final result with new square root
        twodiv *= 2; // update divisor
        result = final / twodiv;
        terms++;
    }
    return 2 / result;
}

int pi_viete_factors(void) {
    return terms;
}
