#include <string.h>
#include "blockchain.h"

/**
 * block_is_valid - verifies that a block is valid
 * @block: pointer to the block to validate
 * @prev_block: pointer to the previous block in the chain (or NULL if genesis)
 *
 * Return: 0 if valid, -1 if invalid
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];

	if (!block)
		return (-1);

	if (!prev_block)
	{
		if (block->info.index != 0)
			return (-1);

		if (memcmp(block, &_genesis, sizeof(block_t)) != 0)
			return (-1);

		return (0);
	}

	if (block->info.index != prev_block->info.index + 1)
		return (-1);

	block_hash(prev_block, hash_buf);
	if (memcmp(prev_block->hash, hash_buf, SHA256_DIGEST_LENGTH) != 0)
		return (-1);

	if (memcmp(block->info.prev_hash, hash_buf, SHA256_DIGEST_LENGTH) != 0)
		return (-1);

	block_hash(block, hash_buf);
	if (memcmp(block->hash, hash_buf, SHA256_DIGEST_LENGTH) != 0)
		return (-1);

	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (-1);

	return (0);
}
