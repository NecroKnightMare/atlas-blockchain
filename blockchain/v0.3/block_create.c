#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "blockchain.h"

/**
 * block_create - Creates and initializes a new block
 * @prev: pointer to the previous block in chain
 * @data: pointer to the data to copy into new block
 * @data_len: number of bytes to copy from @data
 *
 * Return: pointer to the newly created block, NULL on failure
 */
block_t *block_create(block_t const *prev, int8_t const *data,
	uint32_t data_len)
{
	block_t *block = calloc(1, sizeof(block_t));

	if (!block)
		return (NULL);

	block->info.index = prev ? prev->info.index + 1 : 0;
	memcpy(block->info.prev_hash, prev ? prev->hash :
		(uint8_t[SHA256_DIGEST_LENGTH]){0}, SHA256_DIGEST_LENGTH);
	block->info.timestamp = (uint64_t)time(NULL);
	block->info.difficulty = prev ? prev->info.difficulty : 0;
	block->info.nonce = 0;

	block->data.len = data_len > BLOCKCHAIN_DATA_MAX ? BLOCKCHAIN_DATA_MAX :
	data_len;
	memcpy(block->data.buffer, data, block->data.len);

	block->transactions = llist_create(MT_SUPPORT_FALSE);
	if (!block->transactions)
	{
		free(block);
		return (NULL);
	}

	return (block);
}
