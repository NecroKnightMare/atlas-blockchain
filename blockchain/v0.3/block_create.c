#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "blockchain.h"

/**
 * block_create - Creates and initializes a new block
 * @prev: pointer to the previous block in chain
 * @data: pointer to the data to copy into new block
 * @data_len: number of bytes to copy from @data
 *
 * Return: pointer to the newly created block, NULL on failure
 */
block_t *block_create(block_t const *prev, int8_t const *data,
	uint32_t data_len)
{
	blockchain_t *blockchain = calloc(1, sizeof(blockchain_t));
	block_t *genesis;
	llist_t *chain, *unspent;

	if (!blockchain)
		return (NULL);

	chain = llist_create(MT_SUPPORT_FALSE);
	if (!chain)
	{
		free(blockchain);
		return (NULL);
	}

	unspent = llist_create(MT_SUPPORT_FALSE);
	if (!unspent)
	{
		llist_destroy(chain, 1, NULL);
		free(blockchain);
		return (NULL);
	}

	genesis = genesis_block();
	if (!genesis)
	{
		llist_destroy(chain, 1, NULL);
		llist_destroy(unspent, 1, NULL);
		free(blockchain);
		return (NULL);
	}

	if (llist_add_node(chain, genesis) == -1)
	{
		block_destroy(genesis);
		llist_destroy(chain, 1, NULL);
		llist_destroy(unspent, 1, NULL);
		free(blockchain);
		return (NULL);
	}

	blockchain->chain = chain;
	blockchain->unspent = unspent;
	return (blockchain);
	}
