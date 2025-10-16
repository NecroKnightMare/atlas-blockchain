#include "blockchain.h"
#include <string.h>
#include <stdbool.h>

/**
 * block_is_valid - checks if valid against prev block
 * @block: pointer to block to validate
 * @prev_block: pointer to previous
 * Return: 1 if valid, 0 otherwise
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];

	if (!block || !prev_block)
		return (0);

	if (block->info.index != prev_block->info.index + 1)
		return (0);

	if (block->info.timestamp <= prev_block->info.timestamp)
		return (0);

	if (memcmp(block->info.prev_hash, prev_block->hash,
		SHA256_DIGEST_LENGTH) != 0)
			return (0);

	if (!block_hash(block, hash_buf) || memcmp(hash_buf, block->hash,
		SHA256_DIGEST_LENGTH) != 0)
		return (0);

	if (!hash_matches_difficulty(hash_buf, block->info.difficulty))
		return (false);

	if (hash_matches_difficulty(hash_buf, block->info.difficulty) != 0)
		return (0);

	return (1);
}
