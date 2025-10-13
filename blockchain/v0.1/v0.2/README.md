# Blockchain - Block mining
 Amateur
 By: Alexandre Gautier, Software Engineer at Holberton School


[EYNTK] Blockchain - Block mining
Resources
Read or watch:

Everything you need to know - Block mining
Provided libraries and sources
Mining

### General
* What is the purpose of the Block difficulty
* What is the Proof of Work
* How to check that a given hash matches a given difficulty
* How to mine a Block
Why Block mining is brute force only
How to automatically adjust the difficulty
Requirements
General
Allowed editors: vi, vim, emacs
All your files will be compiled on Ubuntu 20.04 LTS
Your C programs and functions will be compiled with gcc 9.* using the flags -Wall -Werror -Wextra, -pedantic -Wno-deprecated-declarations and the linker flags -lssl and -lcrypto
Your library libhblk_crypto.a, located in the crypto folder will be built and linked during correction
All your files should end with a new line
A README.md file, at the root of the folder of the project, is mandatory
Your code should use the Betty style. It will be checked using betty-style.pl and betty-doc.pl
The prototypes of all your functions should be included in your header file called blockchain/v0.2/blockchain.h
All your header files should be include guarded
More Info
Prerequisites
Create the folder blockchain/v0.2/ and copy the following files in it:

blockchain/v0.1/blockchain_create.c
blockchain/v0.1/blockchain_deserialize.c
blockchain/v0.1/blockchain_destroy.c
blockchain/v0.1/blockchain.h
blockchain/v0.1/blockchain_serialize.c
blockchain/v0.1/block_create.c
blockchain/v0.1/block_destroy.c
blockchain/v0.1/block_hash.c
blockchain/v0.1/block_is_valid.c
Modify your code so when a Blockchain is serialized, the version stored is 0.2

Data structures
Please refer to the concept page linked to this project to get familiar with the different data structures used in this project.

Additional information
Unless specified otherwise, you are allowed to use the C standard library
You are free to use any data structure that suits you as long as their purpose is well defined
You are free to print any information on stderr, this stream will be discarded during correction. Since you are going to use these utility functions in your Blockchain project, you are free to manage errors as you wish.
At the end of this project, the structure of your repository should look like this (you may have additional files):
holbertonschool-blockchain
├── blockchain
│   ├── v0.1/
│   └── v0.2
│       ├── blockchain_create.c
│       ├── blockchain_deserialize.c
│       ├── blockchain_destroy.c
│       ├── blockchain_difficulty.c
│       ├── blockchain.h
│       ├── blockchain_serialize.c
│       ├── block_create.c
│       ├── block_destroy.c
│       ├── block_hash.c
│       ├── block_is_valid.c
│       ├── block_mine.c
│       ├── hash_matches_difficulty.c
│       ├── libhblk_blockchain_v0.2.a
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
│           ├── blockchain_difficulty-main.c
│           ├── blockchain_serialize-main.c
│           ├── block_create-main.c
│           ├── block_destroy-main.c
│           ├── block_hash-main.c
│           ├── block_is_valid-main.c
│           ├── block_mine-main.c
│           └── hash_matches_difficulty-main.c
├── crypto/
└── README.md
