#include "mathlib.h"

#include <stdio.h>

// intitizle global var for terms
static int terms = 0;

//approximate the value of π using the Bailey-Borwein-Plouffe formula and track the number of computed terms
double pi_bbp(void) {
    terms = 0; //terms to 0 to reset after each call
    double pi = 0;
    double denominator = 1;
    for (int k = 0; 1.0 / denominator >= EPSILON; k++) {
        double numerator = (4 * 1.0 / (8 * k + 1)) - (2 * 1.0 / (8 * k + 4))
                           - (1 * 1.0 / (8 * k + 5))
                           - (1 * 1.0 / (8 * k + 6)); //calculate numerator using the bbp formula
        pi += numerator / denominator; //add numerator/denominator to pi
        denominator *= 16; //multiply denominator by 16
        terms++; //
    }
    return pi;
}

int pi_bbp_terms(void) {
    return terms;
}
