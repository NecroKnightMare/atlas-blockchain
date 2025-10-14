#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "blockchain.h"

/**
 * block_create - Creates and initializes a new block
 * @prev: pointer to the previous block in the chain
 * @data: pointer to the data to copy into the new block
 * @data_len: number of bytes to copy from @data
 *
 * Return: pointer to the newly created block, or NULL on failure
 */
block_t *block_create(block_t const *prev, int8_t const *data,
	uint32_t data_len)
{
	block_t *block;

	if (!prev || !data)
		return (NULL);

	block = malloc(sizeof(*block));
	if (!block)
		return (NULL);

	block->info.index = prev->info.index + 1;
	block->info.difficulty = 0;
	block->info.timestamp = (uint64_t)time(NULL);
	block->info.nonce = 0;
	memcpy(block->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);

	if (data_len > BLOCKCHAIN_DATA_MAX)
		data_len = BLOCKCHAIN_DATA_MAX;

	memcpy(block->data.buffer, data, data_len);
	block->data.len = data_len;

	memset(block->hash, 0, SHA256_DIGEST_LENGTH);

	return (block);
}
