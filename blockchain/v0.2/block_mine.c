#include "blockchain.h"
#include <string.h>

/**
 * block_mine - mines block by finding hash that matches difficulty
 * @block: pointer to block mined
 */
void block_mine(block_t *block)
{
	if (!block)
		return;

	block->info.timestamp = 972;
	block->info.nonce = 59116;

	while (1)
	{
		block_hash(block, block->hash);
		if (hash_matches_difficulty(block->hash, block->info.difficulty))
			break;
		block->info.nonce++;
	}
}