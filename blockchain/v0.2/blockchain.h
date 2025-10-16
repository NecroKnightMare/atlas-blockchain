#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <stddef.h>
#include "llist.h"
#include <openssl/sha.h>


#define BLOCKCHAIN_DATA_MAX 1024


/*block info struct*/
typedef struct block_info_s
{
	uint32_t index;
	uint32_t difficulty;
	uint64_t timestamp;
	uint64_t nonce;
	uint8_t prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/*Block data*/
typedef struct block_data_s
{
	int8_t buffer[BLOCKCHAIN_DATA_MAX];
	unsigned int len;
} block_data_t;

/*Block struct*/
typedef struct block_s
{
	block_info_t info;
	block_data_t data;
	uint8_t hash[SHA256_DIGEST_LENGTH];
} block_t;

extern const block_t _genesis;

/*Blockchain struct*/
typedef struct blockchain_s
{
	llist_t *chain;
} blockchain_t;

/* v0.1 prototypes */
blockchain_t *blockchain_create(void);
block_t *block_create(block_t const *prev, int8_t const *data,
	uint32_t data_len);
void block_destroy(block_t *block);
void blockchain_destroy(blockchain_t *blockchain);
int blockchain_serialize(blockchain_t const *blockchain,
	char const *path);
uint8_t *block_hash(block_t const *block, uint8_t
	hash_buf[SHA256_DIGEST_LENGTH]);
int blockchain_serialize(blockchain_t const *blockchain, char const *path);
blockchain_t *blockchain_deserialize(char const *path);
int block_is_valid(block_t const *block, block_t const *prev_block);

/*v0.2 prototypes*/
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH], uint32_t difficulty);
void block_mine(block_t *block);

#endif // BLOCKCHAIN_H