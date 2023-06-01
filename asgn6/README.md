# Assignment 6 Lempel-Ziv Compression
Lempel-Ziv LZ78 Compression and Decompression

# Building
$ make\
$ make clean

# Command Usage

## encode.c
- <code>-i [input]</code>: Specify input to compress (stdin by default)
- <code>-o [output]</code>: Specify input to compress (stdout by default)
- <code>-v</code>: Display compression statistics
- <code>-h</code>: Display a help message detailing program usage

## decode.c
- <code>-i [input]</code>: Specify input to decompress (stdin by default)
- <code>-o [output]</code>: Specify input to decompress (stdout by default)
- <code>-v</code>: Display decompression statistics
- <code>-h</code>: Display a help message detailing program usage

# Example
Note: need to make input text file before running
```
./encode -i input -o output -v
```
```
./decode -i output -o output2 -v
```
## Files

### word.{c.h}
source and header file for Word

### trie.{c.h}
source and header file for Trie

### io.{c.h}
source and header file for I/O

### code.h
Header file for code

### endian.h
Header file for Endian functions

### encode.c
Compression Main file

### decode.c 
Decompression Main file

### DESIGN.pdf
Design Document

### WRITEUP.pdf
Writeup Document