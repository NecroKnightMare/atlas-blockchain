#include <string.h>
#include "blockchain.h"

/**
 * block_is_valid - verifies that a block is valid
 * @block: pointer to the block to validate
 * @prev_block: pointer to the previous block in the chain, NULL if genesis
 * @all_unspent: list of all unspent transaction outputs
 *
 * Return: 0 if valid, -1 if invalid
 */
int block_is_valid(block_t const *block, block_t const *prev_block,
	llist_t *all_unspent)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];
	transaction_t *tx;
	int tx_count;

	if (!block || (!prev_block && block->info.index != 0) || !all_unspent)
		return (-1);

	if (!prev_block)
	{
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

	tx_count = llist_size(block->transactions);
	if (tx_count < 1)
		return (-1);

	tx = llist_get_node_at(block->transactions, 0);
	if (!tx || !coinbase_is_valid(tx, block->info.index))
		return (-1);

	for (int i = 1; i < tx_count; i++)
	{
		tx = llist_get_node_at(block->transactions, i);
		if (!tx || !transaction_is_valid(tx, all_unspent))
			return (-1);
	}

	return (0);
}
