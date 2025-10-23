#include "blockchain.h"
#include <string.h>

/**
 * block_is_valid - checks if valid against prev block
 * @block: pointer to block to validate
 * @prev_block: pointer to previous
 * Return: 1 if valid, 0 otherwise
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];
	uint8_t prev_hash_buf[SHA256_DIGEST_LENGTH];

	if (!block)
		return (0);

	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (0);

	if (!prev_block)
	{
		if (block->info.index != 0)
			return (0);
		if (memcmp(block, &_genesis, sizeof(block_t)) != 0)
			return (0);
		return (1);
	}

	if (block->info.index != prev_block->info.index + 1)
		return (0);

	block_hash(prev_block, prev_hash_buf);
	if (memcmp(prev_block->hash, prev_hash_buf, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	if (memcmp(block->info.prev_hash, prev_hash_buf, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	block_hash(block, hash_buf);
	if (memcmp(block->hash, hash_buf, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
		return (0);

	return (1);
}
