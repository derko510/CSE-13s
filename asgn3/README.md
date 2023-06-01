# Assignment 3 Sorting: Putting your affairs in order
A collection of comparison-based sorting algorithms

# Building/Cleaning
$ make\
$ make clean

# Command Usage
- <code>-a</code>: Employs all sorting algorithms.
- <code>-s</code>: Enables Shell Sort
- <code>-b</code>: Enables Batcher Sort
- <code>-q</code>: Enables Quick Sort
- <code>-h</code>: Enables Heap Sort
- <code>-r seed</code>: Set the random seed to seed (Default: 13371453)
- <code>-n size</code>: Set the array size to size (Default: 100)
- <code>-p elements</code>: Print out the number of elements from the array (Default: 100)
- <code>-H</code>: Prints out program usage

# Example 
```
./sorting -a
```

## Files

### DESIGN.pdf
Design Document for the Assignment
### WRITEUP.pdf
Answering the 3 questions in the pdf
### Makefile
Compiles files or cleans derived files
### batcher.{c,h} 
implements batcher Sort
### heap.{c,h}
implements Heap Sort
### shell.{c,h}
implements Shell Sort
### quick.{c,h}
implements Quick Sort
### set.{c,h}
implements set ADT
### stats.{c,h} 
implements the statistics module
### sorting.c 
contains main() and may contain any other functions necessary to complete the assignment.