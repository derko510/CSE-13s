#include "mathlib.h"

#include <stdio.h>

// intitizle global var for terms
static int terms = 1; // 1 becuase the loop starts at 1

double pi_euler(void) {
    terms = 1; //terms to 1 to reset after each call
    double pi = 0.0;
    for (int k = 1; 1.0 / k / k > EPSILON; k++) { // loop until 1/k^2 is less than EPSILON
        pi += 1.0 / k / k; // squares the denomator through dividing same number
        terms++;
    }
    return sqrt_newton(pi * 6); //sqrt of pi * 6 after looping
}

int pi_euler_terms(void) {
    return terms;
}
