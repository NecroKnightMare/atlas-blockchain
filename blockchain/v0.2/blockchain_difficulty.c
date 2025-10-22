#include "blockchain.h"

/**
 * blockchain_difficulty - returns difficulty of last block in blockchain
 * @blockchain: pointer to blockchain
 * Return: difficulty to assign next block
 */
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t const *last_block, *adjust;
	uint32_t expected_time, actual_time;
	size_t size;

	if (!blockchain || !blockchain->chain)
		return (0);

	size = llist_size(blockchain->chain);
	if (size == 0)
		return (0);

	last_block = llist_get_tail(blockchain->chain);
	if (!last_block)
		return (0);

	if (last_block->info.index == 0 || last_block->info.index %
		DIFFICULTY_ADJUSTMENT_INTERVAL != 0)
		return (last_block->info.difficulty);
	adjust = llist_get_node_at(blockchain->chain, size -
		DIFFICULTY_ADJUSTMENT_INTERVAL);

	expected_time = BLOCK_GENERATION_INTERVAL *
	DIFFICULTY_ADJUSTMENT_INTERVAL;
	actual_time = last_block->info.timestamp -
	adjust->info.timestamp;
	
	if (actual_time < expected_time / 2)
		return (last_block->info.difficulty + 1);
	else if (actual_time > expected_time * 2)
		return (last_block->info.difficulty > 0 ?
			last_block->info.difficulty - 1 : 0);
	else
		return (last_block->info.difficulty);
}
