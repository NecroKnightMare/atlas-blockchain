#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <stddef.h>
#include "lllist.h"
#include <openssl/sha.h>

#define BLOCKCHAIN_DATA_MAX 1024
#define GENESIS_INDEX        0
#define GENESIS_DIFFICULTY   0
#define GENESIS_TIMESTAMP    1537578000
#define GENESIS_NONCE        0
#define GENESIS_PREV_HASH    {0}
#define GENESIS_DATA         "Holberton School"
#define GENESIS_DATA_LEN     16
#define GENESIS_HASH         \
    { 0xc5, 0x2c, 0x26, 0xc8, 0xb5, 0x46, 0x16, 0x39, 0x63, 0x5d, 0x8e, \
      0xdf, 0x2a, 0x97, 0xd4, 0x8d, 0x0c, 0x8e, 0x00, 0x09, 0xc8, 0x17, \
      0xf2, 0xb1, 0xd3, 0xd7, 0xff, 0x2f, 0x04, 0x51, 0x58, 0x03 }

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

/*Blockchain struct*/
typedef struct blockchain_s
{
	llist_t *chain;
} blockchain_t;

blockchain_t *blockchain_create(void);


#endif // BLOCKCHAIN_H