#include "blockchain.h"
#include <string.h>
#include <time.h>

/**
 * block_mine - mines block by finding hash that matches difficulty
 * @block: pointer to block mined
 */
void block_mine(block_t *block)
{
	if (!block)
		return;
	block->info.nonce = 0;

	while (1)
	{
		block->info.timestamp = (uint64_t)time(NULL);
		block_hash(block, block->hash);
		if (hash_matches_difficulty(block->hash,
			block->info.difficulty))
			break;
		block->info.nonce++;
	}
}
