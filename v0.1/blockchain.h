#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <stddef.h>
#include "llist.h"
#include <openssl/sha.h>

#define BLOCKCHAIN_DATA_MAX 1024

/*Block struct*/
typedef struct block_s
{
	blocki_info_t info;
	block_data_t data;
	uint8_t hash[SHA256_DIGEST_LENGTH];
} block_t;


blockchain_t *blockchain_create(void);


#endif // BLOCKCHAIN_H