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
	do {
		block->info.nonce++;
		block_hash(block, block->hash);
	} while (!hash_matches_difficulty(block->hash,
		block->info.difficulty));
}
