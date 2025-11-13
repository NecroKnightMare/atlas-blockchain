#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

extern block_t const _genesis;

/**
 * genesis_block - Allocates and returns a copy of the predefined genesis block
 * Return: pointer to a newly allocated genesis block, NULL on failure
 */
block_t *genesis_block(void)
{
	block_t *block = calloc(1, sizeof(block_t));

	if (!block)
		return (NULL);

	memcpy(block, &_genesis, sizeof(block_t));

	block->transactions = llist_create(MT_SUPPORT_FALSE);
	if (!block->transactions)
	{
		free(block);
		return (NULL);
	}

	return (block);
}
