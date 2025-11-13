#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

/**
 * block_hash - Computes the SHA-256 hash of a block, including transactions
 * @block: pointer to the block to hash
 * @hash_buf: buffer to store the resulting hash
 * Return: pointer to hash_buf, NULL on failure
 */
uint8_t *block_hash(block_t const *block,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	size_t i, tx_count, total_size, offset;
	void *buffer;
	transaction_t *tx;

	if (!block || !hash_buf)
		return (NULL);

	tx_count = llist_size(block->transactions);
	total_size = sizeof(block->info) + block->data.len;

	for (i = 0; i < tx_count; i++)
	{
		tx = llist_get_node_at(block->transactions, i);
		if (!tx)
			return (NULL);
		total_size += sizeof(*tx);
	}

	buffer = malloc(total_size);
	if (!buffer)
		return (NULL);

	memcpy(buffer, &block->info, sizeof(block->info));
	memcpy((char *)buffer + sizeof(block->info),
	block->data.buffer, block->data.len);

	offset = sizeof(block->info) + block->data.len;
	for (i = 0; i < tx_count; i++)
	{
		tx = llist_get_node_at(block->transactions, i);
		if (!tx)
		{
			free(buffer);
			return (NULL);
		}
		memcpy((char *)buffer + offset, tx, sizeof(*tx));
		offset += sizeof(*tx);
	}

	sha256(buffer, total_size, hash_buf);
	free(buffer);
	return (hash_buf);
}
