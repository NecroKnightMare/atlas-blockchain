#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <stddef.h>
#include <llist.h>
#include <openssl/sha.h>
#include "block.h"

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
	size_t len;
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

blockchain_t *blockchain_create(void);


#endif // BLOCKCHAIN_H