# Assignment 4 The Game of Life

# Building
$ make\
$ make clean

# Command Usage
- <code>-t</code>: Create your universe as a toroidal
- <code>-s</code>: Silent - do not use animate the evolution
- <code>-n {number}</code>: Number of generations [default: 100]
- <code>-i {file}</code>: Input file [default: stdin]
- <code>-o {file}</code>: Output file [default: stdout]
- <code>-h</code>: Display a help message detailing program usage

# Example
```
./life -s -i lists/101.txt -o output
```
NOTE: lists file needs to be within the file directory for the function to work properply

## Files

### Universe.{c.h}
Source and Header files

### life.c
Main file for command line parsing

### DESIGN.pdf
Design Document

### WRITEUP.pdf
Writeup with errors of my program


