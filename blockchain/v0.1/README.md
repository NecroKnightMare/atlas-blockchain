# Blockchain - Data structures
 Amateur
 By: Alexandre Gautier, Software Engineer at Holberton School
 Weight: 1
 Your score will be updated as you progress.
Description
For this project, we expect you to look at this concept:

[EYNTK] Blockchain - Data structures
Resources
Read or watch:

Everything You Need To Know - Concept page
Provided libraries and sources
Blockchain as a Data Structure

### Learning Objectives
At the end of this project, you are expected to be able to explain to anyone, without the help of Google:

## General
* How a Blockchain is structured
* How Blocks refer to each other
* How immutability is maintained in a Blockchain
* What is the Genesis Block, and why is it important
* How to hash a Block, and what information is stored in the hash

### Requirements

* Allowed editors: vi, vim, emacs
* All your files will be compiled on Ubuntu 20.04 LTS
* Your C programs and functions will be compiled with gcc 9.4.0 using the flags -Wall -Werror -Wextra, -pedantic -Wno-deprecated-declarations and the linker flags -lssl and -lcrypto
* All your files should end with a new line
* A README.md file, at the root of the folder of the project, is mandatory
* Your code should use the Betty style. It will be checked using betty-style.pl and betty-doc.pl
* Your library libhblk_crypto.a, located in the crypto folder will be built and linked during correction
* The prototypes of all your functions should be included in your header file called blockchain/v0.1/blockchain.h
* All your header files should be include guarded

### Additional information

* Unless specified otherwise, you are allowed to use the C standard library
* You are free to use any data structure that suits you as long as their purpose is well defined
* You are free to print any information on stderr, this stream will be discarded during correction. Since you are going to use these utility functions in your Blockchain project, you are free to manage errors as you wish.
* At the end of this project, the structure of your repository should look like this (you may have additional files):

holbertonschool-blockchain
├── blockchain
│   └── v0.1
│       ├── blockchain_create.c
│       ├── blockchain_deserialize.c
│       ├── blockchain_destroy.c
│       ├── blockchain.h
│       ├── blockchain_serialize.c
│       ├── block_create.c
│       ├── block_destroy.c
│       ├── block_hash.c
│       ├── block_is_valid.c
│       ├── libhblk_blockchain_v0.1.a
│       ├── Makefile
│       ├── provided
│       │   ├── _blockchain_destroy.c
│       │   ├── _blockchain_print.c
│       │   ├── _endianness.c
│       │   ├── endianness.h
│       │   └── _genesis.c
│       └── test
│           ├── blockchain_create-main.c
│           ├── blockchain_deserialize-main.c
│           ├── blockchain_destroy-main.c
│           ├── blockchain_serialize-main.c
│           ├── block_create-main.c
│           ├── block_destroy-main.c
│           ├── block_hash-main.c
│           └── block_is_valid-main.c
├── crypto/
└── README.md