#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

extern block_t const _genesis;

/**
 * blockchain_create - Creates new blockchain and initializes the genesis block
 * Return: pointer to the new blockchain NULL on failure
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain = calloc(1, sizeof(blockchain_t));
	block_t *genesis;

	if (!blockchain)
		return (NULL);

	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain)
	{
		free(blockchain);
		return (NULL);
	}

	blockchain->unspent = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->unspent)
	{
		llist_destroy(blockchain->chain, 1, NULL);
		free(blockchain);
		return (NULL);
	}

	genesis = calloc(1, sizeof(block_t));
	if (!genesis)
	{
		llist_destroy(blockchain->chain, 1, NULL);
		llist_destroy(blockchain->unspent, 1, NULL);
		free(blockchain);
		return (NULL);
	}

	memcpy(genesis, &_genesis, sizeof(block_t));
	genesis->transactions = llist_create(MT_SUPPORT_FALSE);
	if (!genesis->transactions)
	{
		free(genesis);
		llist_destroy(blockchain->chain, 1, NULL);
		llist_destroy(blockchain->unspent, 1, NULL);
		free(blockchain);
		return (NULL);
	}

	if (llist_add_node(blockchain->chain, genesis, ADD_NODE_REAR) == -1)
	{
		block_destroy(genesis);
		llist_destroy(blockchain->chain, 1, NULL);
		llist_destroy(blockchain->unspent, 1, NULL);
		free(blockchain);
		return (NULL);
	}

	return (blockchain);
}
