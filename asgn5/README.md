# Assignment 5: Public Key Cryptography

# Building/Cleaning
$ make\
$ make clean\
$ make cleankeys 

# Command Usage

## Keygen
- <code>-b</code>: Specifies the minimum bits needed for the public modulus n (default: 256)
- <code>-i</code>: Specifies the number of Miller-Rabin iterations for testing primes (default: 50)
- <code>-n [pbfile]</code>: Specifies the public key file (default: ss.pub)
- <code>-d [pvfile]</code>: Specifies the private key file (default: ss.priv)
- <code>-s</code>: Specifies the random seed for the random state initialization (default: the seconds since the UNIX epoch, given by time(NULL))
- <code>-v</code>: Enables verbose output
- <code>-h</code>: Displays program synopsis and usage
```
./keygen -v
```
## Encrypt
- <code>-i [infile]</code>: Read input from <infile>. Default: standard input
- <code>-o [outfile]</code>: Write output to <outfile>. Default: standard output
- <code>-n [keyfile]</code>: Public key is in <keyfile>. Default: ss.pub
- <code>-v</code>: Enables verbose output
- <code>-h</code>: Displays program synopsis and usage
```
./encrypt -v
```
## Decrypt
- <code>-i [infile]</code>: Read input from <infile>. Default: standard input
- <code>-o [outfile]</code>: Write output to <outfile>. Default: standard output
- <code>-n [keyfile]</code>: Private key is in <keyfile>. Default: ss.pub
- <code>-v</code>: Enables verbose output
- <code>-h</code>: Displays program synopsis and usage
```
./decrypt -v
```

## Files

### keygen.c
Main file of key generator

### encrypt.c
main file for encryptor

### decrypt.c
main file for decryptor

### numtheory.{c,h}
source and header file

### randstate.{c,h}
source and header file

### ss.{c.h}
source and header file

### DESIGN.pdf
design document

### WRITEUP.pdf
writeup document
