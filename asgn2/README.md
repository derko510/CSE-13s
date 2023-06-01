# Assignment 2: A Slice of π
Creating our own math functions that will be almost identical precision to the original C math library. 

# Building
$ make\
$ make clean

# Command Usage
- <code>-a</code>: Run all tests
- <code>-e</code>: Runs e approximation test
- <code>-b</code>: Runs Bailey-Borwein-Plouffe π approximation test
- <code>-m</code>: Runs Madhava π approximation test
- <code>-r</code>: Runs Euler sequence π approximation test
- <code>-v</code>: Runs Viète π approximation test
- <code>-n</code>: Runs Newton-Raphson square root approximation tests
- <code>-s</code>: Enable printing of statistics to see computed terms and factors for each tested function
- <code>-h</code>: Display a help message detailing program usage

# Example
```
./mathlib-test -a
```

## Files

### e.c
implementation of the Taylor series to approximate Euler’s number e

### bbp.c
implementation of the Bailey-Borwein-Plouffe formula to approximate pi

### madhava.c
implementation of the Madhava series to approximate pi 

### euler.c
implementation of the Euler's solution to approximate pi

### viete.c
implementation of the viete formula to approximate pi

### newton.c
implementation of the square root approximation using Newton’s method \
used Long's implementation

### mathlib-test.c
contains the main() function used to parse tests

### mathlib.h
resource header file

## Errors/Bugs
For all the functions except newton. The main function calls the each test and when returning the terms it would add up the total terms and without the terms = 0/1 it would x2 the total terms. The output of the terms are correct but something is calling the test functions twice.
