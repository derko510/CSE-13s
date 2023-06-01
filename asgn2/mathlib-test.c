#include "mathlib.h"

#include <math.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "aebmrvnsh"

void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "    A test harness for the small numerical library.\n\n"
        "USAGE\n"
        "    %s [-aebmrvnsh]\n\n"
        "OPTIONS\n"
        "   -a     Runs all tests.\n"
        "   -e     Runs e test.\n"
        "   -b     Runs BBP pi test.\n"
        "   -m     Runs Madhava pi test.\n"
        "   -r     Runs Euler pi test.\n"
        "   -v     Runs Viete pi test.\n"
        "   -n     Runs Newton square root tests.\n"
        "   -s     Print verbose statistics.\n"
        "   -h     Display program synopsis and usage.\n",
        exec);
}

// print functions to make cases cleaner
void print_e(void) {
    printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", e(), M_E, e() - M_E);
}

void print_bbp(void) {
    printf(
        "pi_bbp() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_bbp(), M_PI, pi_bbp() - M_PI);
}

void print_madhava(void) {
    printf("pi_madhava() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_madhava(), M_PI,
        pi_madhava() - M_PI);
}

void print_euler(void) {
    printf("pi_euler() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_euler(), M_PI,
        pi_euler() - M_PI);
}

void print_viete(void) {
    printf("pi_viete() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_viete(), M_PI,
        pi_viete() - M_PI);
}

int main(int argc, char **argv) {
    int opt = 0;

    //flags for all tests to run once
    int e_flag = 0;
    int bbp_flag = 0;
    int madhava_flag = 0;
    int euler_flag = 0;
    int viete_flag = 0;
    int newton_flag = 0;

    // help flag to know when to run usage
    int help_flag = 0;

    // verbose
    int verbose = 0;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a': // run all the tests
            e_flag = 1;
            bbp_flag = 1;
            madhava_flag = 1;
            euler_flag = 1;
            viete_flag = 1;
            newton_flag = 1;
            help_flag = 1;
            break;
        case 'e': // run e test
            e_flag = 1;
            help_flag = 1;
            break;
        case 'b': // run BBP pi test
            bbp_flag = 1;
            help_flag = 1;
            break;
        case 'm': // run Madhava pi test
            madhava_flag = 1;
            help_flag = 1;
            break;
        case 'r': // run Euler Pi test
            euler_flag = 1;
            help_flag = 1;
            break;
        case 'v': // run Viete pi test
            viete_flag = 1;
            help_flag = 1;
            break;
        case 'n': // runs newton sqrt tests
            newton_flag = 1;
            help_flag = 1;
            break;
        case 's': // print verbose
            verbose = 1;
            break;
        default: usage(argv[0]); return -1;
        }
    }

    if (e_flag == 1) {
        print_e();
        if (verbose == 1) {
            printf("e() terms = %d\n", e_terms());
        }
    }

    if (euler_flag == 1) {
        print_euler();
        if (verbose == 1) {
            printf("pi_euler() terms = %d\n", pi_euler_terms());
        }
    }

    if (bbp_flag == 1) {
        print_bbp();
        if (verbose == 1) {
            printf("pi_bbp() terms = %d\n", pi_bbp_terms());
        }
    }

    if (madhava_flag == 1) {
        print_madhava();
        if (verbose == 1) {
            printf("pi_madhava() terms = %d\n", pi_madhava_terms());
        }
    }

    if (viete_flag == 1) {
        print_viete();
        if (verbose == 1) {
            printf("pi_viete() terms = %d\n", pi_viete_factors());
        }
    }

    if (newton_flag == 1) {
        for (double i = 0; i <= 10; i += 0.1) { // 0 - 10 with 0.1 increments
            printf("sqrt_newton(%lf) = %16.15lf, sqrt(%lf) = %16.15lf, diff = %16.15lf\n", i,
                sqrt_newton(i), i, sqrt(i), sqrt_newton(i) - sqrt(i));
            if (verbose == 1) {
                printf("sqrt_newton() terms = %d\n", sqrt_newton_iters());
            }
        }
    }

    // prints usage if wrong no switch case was entered
    if (help_flag == 0) {
        usage(argv[0]);
        return -1; // error due to not correct parsing
    }

    return 0;
}
